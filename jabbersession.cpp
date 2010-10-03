#include <iostream>
#include <unistd.h>
#include <sstream>
#include <iomanip>

#include "jabbersession.hpp"
#include "iq-stanza.hpp"
#include "presence-stanza.hpp"
#include "message-stanza.hpp"

#define BUFFERLENGTH 2000

JabberSession::JabberSession(const std::string &host, unsigned short port, bool isSecure) : m_s(host, port, isSecure) {
  m_state = Connected;
  m_depth = 0;
  m_idCount = 0;
  pthread_mutex_init(&m_stateMutex, NULL);
  m_node = NULL;
  m_iqContext = NULL;
  m_presenceHandler = NULL;
  m_presenceContext = NULL;
  m_messageHandler = NULL;
  m_messageContext = NULL;

  if (0 == pthread_create(&m_listenerThread, NULL, ListenerThreadFunction, this)) {
    // SYZYGY -- RFC 3920 specifies an "xml:lang" attribute and a "version" attribute
    // SYZYGY -- I need to configure the "to" somehow
    std::string xml = "<?xml version='1.0'?><stream:stream to='jabber.brokersys.com' xmlns='jabber:client' xmlns:stream='http://etherx.jabber.org/streams' version='1.0'>";
    m_s.Send(xml);
    pthread_mutex_lock(&m_stateMutex);
    while (m_state < GotStreamTag) {
      pthread_mutex_unlock(&m_stateMutex);
      usleep(500);
      pthread_mutex_lock(&m_stateMutex);
    }
    pthread_mutex_unlock(&m_stateMutex);
  }
}


JabberSession::~JabberSession(void) {
  void *listenerData;

  pthread_mutex_lock(&m_stateMutex);
  m_state = Closing;
  pthread_mutex_unlock(&m_stateMutex);
  std::string xml = "</stream:stream>";
  m_s.Send(xml);
  pthread_join(m_listenerThread, &listenerData);
}

void JabberSession::on_start_element(const Glib::ustring &name, const AttributeList &attributes) {
  pthread_mutex_lock(&m_stateMutex);
  std::cout << "on_start_element(): " << name << std::endl;

  if (0 == m_depth) {
    if ((Connected == m_state) && ("stream:stream" == name)) {
      bool has_id = false;
      bool has_from = false;

      for(xmlpp::SaxParser::AttributeList::const_iterator i = attributes.begin(); i != attributes.end(); ++i) {
	if (i->name == "xmlns:stream") {
	  std::cout << "   The stream namespace is \"" << i->value << "\"" << std::endl;
	}
	if (i->name == "id") {
	  std::cout << "   The session identifier is \"" << i->value << "\"" << std::endl;
	  has_id = true;
	  m_sessionIdentifier = i->value;
	}
	if (i->name == "xmlns") {
	  std::cout << "   Declaring XML namespace \"" << i->value << "\"" << std::endl;
	}
	if (i->name == "from") {
	  has_from = true;
	  std::cout << "   The logical server name is \"" << i->value << "\"" << std::endl;
	  m_logicalServer = i->value;
	}
      }
      if (has_from and has_id) {
	m_state = GotStreamTag;
      }
      else {
	m_state = Error;
      }
    }
    else {
      m_state = Error;
    }
  }
  else {
    if (1 == m_depth) {
      if (("message" == name) || ("presence" == name) || ("iq" == name)) {
	m_node = new JabberElementNode(NULL, name, attributes);
      }
      else {
	m_state = Error;
      }
    }
    else {
      // std::cout << "Recursing into node " << name << std::endl;
      JabberElementNode *temp = new JabberElementNode(m_node, name, attributes);
      m_node->m_children.push_back(temp);
      m_node = temp;
    }
  }
  ++m_depth;
  pthread_mutex_unlock(&m_stateMutex);
}


void JabberSession::on_end_element(const Glib::ustring &name) {
  pthread_mutex_lock(&m_stateMutex);
  std::cout << "on_end_element(): " << name << std::endl;
  --m_depth;
  if (NULL != m_node->m_parent) {
    m_node = m_node->m_parent;
  }
  if (1 == m_depth) {
    bool isResponse = false;

    Stanza *message = Stanza::parse(m_node);
    m_node = NULL;
    isResponse = (NULL != message->Type()) && (("result" == *message->Type()) || ("error" == *message->Type()));

    if (isResponse) {
      if (NULL != message->Id()) {
	jabberEventMap_t::iterator i = m_jabberEvents.find(*message->Id());
	if (m_jabberEvents.end() != i) {
	  i->second->s = message;
	  pthread_mutex_unlock(&i->second->c);
	  m_jabberEvents.erase(i);
	}
      }
    }
    else {
      const IqStanza *iqMessage = dynamic_cast<IqStanza *>(message);
      const PresenceStanza *presenceMessage = dynamic_cast<PresenceStanza *>(message);
      const MessageStanza *messageMessage = dynamic_cast<MessageStanza *>(message);
      if (NULL != iqMessage) {
	if (NULL != message->Namespace()) {
	  iqHandlerMap_t::iterator i = m_iqHandlers.find(*iqMessage->Namespace());
	  if (m_iqHandlers.end() != i) {
	    m_iqHandlers[*iqMessage->Namespace()](m_iqContext, *iqMessage, this);
	  }
	}
      }
      if (NULL != presenceMessage) {
	if (NULL != m_presenceHandler) {
	  (*m_presenceHandler)(m_presenceContext, *presenceMessage, this);
	}
      }
      if (NULL != messageMessage) {
	if (NULL != m_messageHandler) {
	  (*m_messageHandler)(m_messageContext, *messageMessage, this);
	}
      }
    }
  }
  pthread_mutex_unlock(&m_stateMutex);
}


void JabberSession::on_characters(const Glib::ustring &characters) {
  pthread_mutex_lock(&m_stateMutex);
  std::cout << "on_characters(): " << characters << std::endl;
  if (NULL != m_node) {
    JabberTextNode *node = new JabberTextNode(m_node, characters);
    m_node->m_children.push_back(node);
  }
  pthread_mutex_unlock(&m_stateMutex);
}


void JabberSession::on_comment(const Glib::ustring &text) {
  pthread_mutex_lock(&m_stateMutex);
  std::cout << "on_comment(): " << text << std::endl;
  pthread_mutex_unlock(&m_stateMutex);
}


void JabberSession::on_warning(const Glib::ustring &text) {
  pthread_mutex_lock(&m_stateMutex);
  std::cout << "on_warning(): " << text << std::endl;
  pthread_mutex_unlock(&m_stateMutex);
}


void JabberSession::on_error(const Glib::ustring &text) {
  pthread_mutex_lock(&m_stateMutex);
  std::cout << "on_error(): " << text << std::endl;
  pthread_mutex_unlock(&m_stateMutex);
}


void JabberSession::on_fatal_error(const Glib::ustring &text) {
  pthread_mutex_lock(&m_stateMutex);
  std::cout << "on_fatal_error(): " << text << std::endl;
  pthread_mutex_unlock(&m_stateMutex);
}

void *JabberSession::ListenerThreadFunction(void *data) {
  JabberSession *t = (JabberSession *)data;
  uint8_t buff[BUFFERLENGTH];

  pthread_mutex_lock(&t->m_stateMutex);
  while (t->m_state < Closing) {
    pthread_mutex_unlock(&t->m_stateMutex);
    // Get the state mutex
    long l = t->m_s.Receive(buff, BUFFERLENGTH);
    buff[l] = '\0';
    std::cout << "Received " << l << " chars \"" << buff << "\"" << std::endl;
    if (0 < l) {
      Glib::ustring input((char *)buff, l);
      t->parse_chunk(input);
      pthread_mutex_lock(&t->m_stateMutex);
    }
    else {
      if (0 > l) {
	// Get the state mutex
	pthread_mutex_lock(&t->m_stateMutex);
	t->m_state = Closed;
      }
      else {
	pthread_mutex_lock(&t->m_stateMutex);
      }
    }
  }
  pthread_mutex_unlock(&t->m_stateMutex);
  t->finish_chunk_parsing();

  return NULL;
}


const Stanza *JabberSession::SendMessage(const Stanza &request,  bool expectingReply) {
  jabberEvent_t *e = NULL;
  // SYZYGY -- I need to rework this logic because currently there's no way to send a message that
  // SYZYGY -- doesn't have an ID
  const std::string *xml;
  if (expectingReply) {
    pthread_mutex_lock(&m_stateMutex);
    unsigned long local_count = m_idCount++;
    e = new(jabberEvent_t);
    std::ostringstream id;
    id << std::hex << std::setw(8) << std::setfill('0') << local_count;
    std::string *id_string = new std::string(id.str());
    xml = request.render(id_string);
    delete id_string;
    pthread_mutex_init(&e->c, NULL);
    pthread_mutex_lock(&e->c);
    m_jabberEvents[id.str()] = e;
    pthread_mutex_unlock(&m_stateMutex);
  }
  else {
    xml = request.render(NULL);
  }
  m_s.Send(*xml);
  if (expectingReply) {
    pthread_mutex_lock(&e->c);

    Stanza *result = e->s;
    delete e;
    return result;
  }

  // It falls through to here if it's not expecting a reply.  Since I'm not expecting
  // a reply, I can return a pointer to nothing
  return NULL;
}


void JabberSession::Register(void *context, iqHandler_t iqHandler, const std::string &name_space) {
  m_iqContext = context;
  if (NULL != iqHandler) {
    m_iqHandlers[name_space] = iqHandler;
  }
  else {
    iqHandlerMap_t::iterator i = m_iqHandlers.find(name_space);
    if (m_iqHandlers.end() != i) {
      m_iqHandlers.erase(i);
    }
  }
}


void JabberSession::Register(void *context, presenceHandler_t presenceHandler) {
  m_presenceHandler = presenceHandler;
  m_presenceContext = context;
}


void JabberSession::Register(void *context, messageHandler_t messageHandler) {
  m_messageHandler = messageHandler;
  m_messageContext = context;
}

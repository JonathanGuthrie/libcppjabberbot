#include <sstream>
// #include <iostream>
#include <cstdlib>

#include "message-stanza.hpp"

typedef std::map<std::string, MessageStanza::MessageTypes> typeMap_t;
static typeMap_t *typeMap = NULL;

MessageStanza::MessageStanza(void) {
  m_type = Normal;
  m_body = NULL;
  m_subject = NULL;
  m_html = NULL;
  m_thread = NULL;
  if (NULL == typeMap) {
    typeMap = new typeMap_t;
    (*typeMap)["normal"]    = Normal;
    (*typeMap)["chat"]      = Chat;
    (*typeMap)["groupchat"] = Groupchat;
    (*typeMap)["headline"]  = Headline;
    (*typeMap)["error"]     = Error;
  }
}


MessageStanza::~MessageStanza(void) {
  delete m_body;
  delete m_subject;
  delete m_html;
  delete m_thread;
}


const std::string *MessageStanza::render(const std::string *id) const {
  std::ostringstream message;

  if (NULL != m_body) {
    message << "<body>" << *m_body << "</body>";
  }
  if (NULL != m_subject) {
    message << "<subject>" << *m_subject << "</subject>";
  }
  if (NULL != m_html) {
    message << "<html>" << *m_html << "</html>";
  }
  if (NULL != m_thread) {
    message << "<thread>" << *m_thread << "</thread>";
  }
  return renderStanza(id, "message", message.str());
}


Stanza *MessageStanza::parse(const JabberElementNode *root) {
  MessageStanza *result = new MessageStanza();

  result->Type(Normal);
  for(xmlpp::SaxParser::AttributeList::const_iterator i = root->m_attributes.begin(); i != root->m_attributes.end(); ++i) {
    if ("type" == i->name) {
      typeMap_t::const_iterator j = typeMap->find(i->value);
      if (j != typeMap->end()) {
	result->Type((*typeMap)[i->value]);
      }
      else {
	result->Type(UnknownMessage);
      }
    }
    if ("id" == i->name) {
      result->Id(i->value);
    }
    if ("from" == i->name) {
      result->From(i->value);
    }
  }
  for (jabberNodeList_t::const_iterator i=root->m_children.begin(); i != root->m_children.end(); ++i) {
    const JabberElementNode *node = dynamic_cast<JabberElementNode *>(*i);
    if ((NULL != node) && ("body" == node->m_name)) {
      jabberNodeList_t::const_iterator j=node->m_children.begin();
      const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
      if (NULL != text) {
	result->Body(text->m_data);
      }
    }
    if ((NULL != node) && ("subject" == node->m_name)) {
      jabberNodeList_t::const_iterator j=node->m_children.begin();
      const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
      if (NULL != text) {
	result->Subject(text->m_data);
      }
    }
    if ((NULL != node) && ("html" == node->m_name)) {
      jabberNodeList_t::const_iterator j=node->m_children.begin();
      const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
      if (NULL != text) {
	result->Html(text->m_data);
      }
    }
    if ((NULL != node) && ("thread" == node->m_name)) {
      jabberNodeList_t::const_iterator j=node->m_children.begin();
      const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
      if (NULL != text) {
	result->Thread(text->m_data);
      }
    }
  }
  return result;
}


void MessageStanza::Type(MessageTypes type) {
  m_type = type;
  switch(type) {
  case Chat:
    ((Stanza *)this)->Type(new std::string("chat"));
    break;

  case Groupchat:
    ((Stanza *)this)->Type(new std::string("groupchat"));
    break;

  case Headline:
    ((Stanza *)this)->Type(new std::string("headline"));
    break;

  case Error:
    ((Stanza *)this)->Type(new std::string("error"));
    break;

  default:
    ((Stanza *)this)->Type(NULL);
    break;
  }
}


void MessageStanza::Body(const std::string *body) {
  delete m_body;
  m_body = body;
}


void MessageStanza::Body(const std::string &body) {
  Body(new std::string(body));
}


void MessageStanza::Subject(const std::string *subject) {
  delete m_subject;
  m_subject = subject;
}


void MessageStanza::Subject(const std::string &subject) {
  Subject(new std::string(subject));
}


void MessageStanza::Html(const std::string *html) {
  delete m_html;
  m_html = html;
}


void MessageStanza::Html(const std::string &html) {
  Html(new std::string(html));
}


void MessageStanza::Thread(const std::string *thread) {
  delete m_thread;
  m_thread = thread;
}


void MessageStanza::Thread(const std::string &thread) {
  Thread(new std::string(thread));
}


MessageStanza::MessageTypes MessageStanza::Type(void) const {
  return m_type;
}


const std::string *MessageStanza::Body(void) const {
  return m_body;
}


const std::string *MessageStanza::Subject(void) const {
  return m_subject;
}


const std::string *MessageStanza::Html(void) const {
  return m_html;
}


const std::string *MessageStanza::Thread(void) const {
  return m_thread;
}



#include <sstream>
// #include <iostream>
#include <cstdlib>

#include "presence-stanza.hpp"

typedef std::map<std::string, PresenceStanza::PresenceTypes> typeMap_t;
typedef std::map<std::string, PresenceStanza::ShowTypes> showMap_t;
static typeMap_t *typeMap = NULL;
static showMap_t *showMap = NULL;

PresenceStanza::PresenceStanza(void) {
  m_type = Available;
  m_show = Normal;
  m_status = NULL;
  m_priority = -1;
  if (NULL == typeMap) {
    typeMap = new typeMap_t;
    (*typeMap)["available"]    = Available;
    (*typeMap)["unavailable"]  = Unavailable;
    (*typeMap)["probe"]        = Probe;
    (*typeMap)["subscribe"]    = Subscribe;
    (*typeMap)["unsubscribe"]  = Unsubscribe;
    (*typeMap)["subscribed"]   = Subscribed;
    (*typeMap)["unsubscribed"] = Unsubscribed;
  }
  if (NULL == showMap) {
    showMap = new showMap_t;
    (*showMap)["normal"] = Normal;
    (*showMap)["away"] = Away;
    (*showMap)["chat"] = Chat;
    (*showMap)["dnd"] = Dnd;
    (*showMap)["xa"] = Xa;
  }
}


PresenceStanza::~PresenceStanza(void) {
  delete m_status;
}


const std::string *PresenceStanza::render(const std::string *id) const {
  std::ostringstream announcement;

  if (NULL != m_status) {
    announcement << "<status>" << *m_status << "</status>";
  }
  if (Normal != m_show) {
    announcement << "<show>";
    switch (m_show) {
    case Away:
      announcement << "away";
      break;

    case Chat:
      announcement << "chat";
      break;

    case Dnd:
      announcement << "dnd";
      break;

    default:
      announcement << "xa";
      break;
    }
    announcement << "</show>";
  }
  if (0 < m_priority) {
    announcement << "<priority>" << m_priority << "</priority>";
  }
  return renderStanza(id, "presence", announcement.str());
}


Stanza *PresenceStanza::parse(const JabberElementNode *root) {
  PresenceStanza *result = new PresenceStanza();

  result->Type(Available);
  for(xmlpp::SaxParser::AttributeList::const_iterator i = root->m_attributes.begin(); i != root->m_attributes.end(); ++i) {
    if ("type" == i->name) {
      typeMap_t::const_iterator j = typeMap->find(i->value);
      if (j != typeMap->end()) {
	result->Type((*typeMap)[i->value]);
      }
      else {
	result->Type(UnknownType);
      }
    }
    if ("id" == i->name) {
      result->Id(i->value);
    }
    if ("from" == i->name) {
      result->From(i->value);
    }
  }
  result->Show(Normal);
  for (jabberNodeList_t::const_iterator i=root->m_children.begin(); i != root->m_children.end(); ++i) {
    const JabberElementNode *node = dynamic_cast<JabberElementNode *>(*i);
    if ((NULL != node) && ("show" == node->m_name)) {
      jabberNodeList_t::const_iterator j=node->m_children.begin();
      const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
      if (NULL != text) {
	typeMap_t::const_iterator j = typeMap->find(text->m_data);
	if (j != typeMap->end()) {
	  result->Show((*showMap)[text->m_data]);
	}
	else {
	  result->Show(UnknownShow);
	}
      }
    }
    if ((NULL != node) && ("status" == node->m_name)) {
      jabberNodeList_t::const_iterator j=node->m_children.begin();
      if (node->m_children.end() != j) {
	const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
	if (NULL != text) {
	  result->Status(text->m_data);
	}
      }
    }
    if ((NULL != node) && ("priority" == node->m_name)) {
      jabberNodeList_t::const_iterator j=node->m_children.begin();
      if (node->m_children.end() != j) {
	const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
	if (NULL != text) {
	  result->Priority(strtol(text->m_data.c_str(), NULL, 10));
	}
      }
    }
  }
  return result;
}


void PresenceStanza::Type(PresenceTypes type) {
  m_type = type;
  switch(type) {
  case Unavailable:
    ((Stanza *)this)->Type(new std::string("unavailable"));
    break;

  case Probe:
    ((Stanza *)this)->Type(new std::string("probe"));
    break;

  case Subscribe:
    ((Stanza *)this)->Type(new std::string("subscribe"));
    break;

  case Unsubscribe:
    ((Stanza *)this)->Type(new std::string("unsubscribe"));
    break;

  case Subscribed:
    ((Stanza *)this)->Type(new std::string("subscribed"));
    break;

  case Unsubscribed:
    ((Stanza *)this)->Type(new std::string("unsubscribed"));
    break;

  default:
    ((Stanza *)this)->Type(NULL);
    break;
  }
}


void PresenceStanza::Show(ShowTypes show) {
  m_show = show;
}


void PresenceStanza::Status(const std::string *status) {
  delete m_status;
  m_status = status;
}


void PresenceStanza::Status(const std::string &status) {
  Status(new std::string(status));
}


void PresenceStanza::Priority(int priority) {
  m_priority = priority;
}


PresenceStanza::PresenceTypes PresenceStanza::Type(void) const {
  return m_type;
}


PresenceStanza::ShowTypes PresenceStanza::Show(void) const {
  return m_show;
}


const std::string *PresenceStanza::Status(void) const {
  return m_status;
}


int PresenceStanza::Priority(void) const {
  return m_priority;
}

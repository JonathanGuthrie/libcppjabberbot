#include <sstream>
#include <iostream>
#include <cstdlib>

#include "presence-stanza.hpp"

PresenceStanza::PresenceStanza(void) {
  m_type = Available;
  m_show = NULL;
  m_status = NULL;
  m_priority = -1;
}


PresenceStanza::~PresenceStanza(void) {
  delete m_show;
  delete m_status;
}


const std::string *PresenceStanza::render(const std::string *id) const {
  std::ostringstream announcement;

  if (NULL != m_status) {
    announcement << "<status>" << *m_status << "</status>";
  }
  if (NULL != m_show) {
    announcement << "<show>" << *m_show << "</show>";
  }
  if (0 < m_priority) {
    announcement << "<priority>" << m_priority << "</priority>";
  }
  return renderStanza(id, "presence", announcement.str());
}


Stanza *PresenceStanza::parse(const JabberElementNode *root) {
  return new PresenceStanza();
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


void PresenceStanza::Show(const std::string *show) {
  delete m_show;
  m_show = show;
}


void PresenceStanza::Show(const std::string &show) {
  Show(new std::string(show));
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


const std::string *PresenceStanza::Show(void) const {
  return m_show;
}


const std::string *PresenceStanza::Status(void) const {
  return m_status;
}


int PresenceStanza::Priority(void) const {
  return m_priority;
}

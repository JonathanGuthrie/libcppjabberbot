#include <sstream>

#include "stanza.hpp"

Stanza::Stanza(void) {
  m_to = NULL;
  m_from = NULL;
  m_type = NULL;
  m_id = NULL;
}

Stanza::~Stanza(void) {
}


void Stanza::To(const std::string &to) {
  if (NULL != m_to) {
    delete m_to;
  }
  m_to = new std::string(to);
}


void Stanza::From(const std::string &from) {
  if (NULL != m_from) {
    delete m_from;
  }
  m_from = new std::string(from);
}


void Stanza::Type(const std::string &type) {
  if (NULL != m_type) {
    delete m_type;
  }
  m_type = new std::string(type);
}


void Stanza::Id(const std::string &id) {
  if (NULL != m_id) {
    delete m_id;
  }
  m_id = new std::string(id);
}


const std::string &Stanza::To(void) const {
  return *m_to;
}


const std::string &Stanza::From(void) const {
  return *m_from;
}


const std::string &Stanza::Type(void) const {
  return *m_type;
}


const std::string &Stanza::Id(void) const  {
  return *m_id;
}


const std::string *Stanza::renderStanza(const std::string *id, const std::string &tag, const std::string &body) const {
  std::ostringstream result;

  result << "<" << tag;
  if (NULL != id) {
    result << " id='" << *id << "'";
  }
  if (NULL != m_to) {
    result << " to='" << *m_to << "'";
  }
  if (NULL != m_from) {
    result << " from='" << *m_from << "'";
  }
  if (NULL != m_type) {
    result << " type='" << *m_type << "'";
  }
  result << ">" << body << "</" << tag << ">";
  return new std::string(result.str());
}

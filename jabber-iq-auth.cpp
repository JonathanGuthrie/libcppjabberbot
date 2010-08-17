#include <sstream>

#include "jabber-iq-auth.hpp"

JabberIqAuth::JabberIqAuth(const std::string &username, const std::string &password, const std::string &resource) {
  Type("set");
  m_username = new std::string(username);
  m_password = new std::string(password);
  if ("" != resource) {
    m_resource = new std::string(resource);
  }
  else {
    m_resource = NULL;
  }
}


JabberIqAuth::~JabberIqAuth(void) {
  delete m_username;
  delete m_password;
  delete m_resource;
}


const std::string *JabberIqAuth::render(const std::string *id) const {
  std::ostringstream body;

  body << "<username>" << *m_username << "</username>";
  body << "<password>" << *m_password << "</password>";
  if (NULL != m_resource) {
    body << "<resource>" << *m_resource << "</resource>";
  }
  return renderIqStanza(id, "jabber:iq:auth", body.str());
}

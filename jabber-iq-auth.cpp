#include <sstream>

#include "jabber-iq-auth.hpp"

JabberIqAuth::JabberIqAuth(const std::string &username, const std::string &password, const std::string &resource) {
  Namespace("jabber:iq:auth");
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
  return renderIqStanza(id, Namespace(), body.str());
}


Stanza *JabberIqAuth::parse(const JabberElementNode *query, const JabberElementNode *error) {
  Stanza *result = NULL;
  const std::string *username = NULL;
  const std::string *password = NULL;
  const std::string *resource = NULL;

  for (jabberNodeList_t::const_iterator i=query->m_children.begin(); i != query->m_children.end(); ++i) {
    const JabberElementNode *node = dynamic_cast<JabberElementNode *>(*i);
    if (NULL != node) {
      if ("username" == node->m_name) {
	jabberNodeList_t::const_iterator j=node->m_children.begin();
	const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
	if (NULL != text) {
	  username = new std::string(text->m_data);
	}
      }
      if ("password" == node->m_name) {
	jabberNodeList_t::const_iterator j=node->m_children.begin();
	const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
	if (NULL != text) {
	  password = new std::string(text->m_data);
	}
      }
      if ("resource" == node->m_name) {
	jabberNodeList_t::const_iterator j=node->m_children.begin();
	const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
	if (NULL != text) {
	  resource = new std::string(text->m_data);
	}
      }
    }
  }
  if ((NULL != username) && (NULL != password) && (NULL != resource)) {
    result = new JabberIqAuth(*username, *password, *resource);
    // SYZYGY -- I may do other things here, like record the error
  }
  else {
    // TODO -- decide how, if at all, I handle this error
    // 401 means invalid username or password
  }
  return result;
}

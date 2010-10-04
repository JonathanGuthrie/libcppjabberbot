/*
 * Copyright 2010 Jonathan R. Guthrie
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <sstream>

#include "jabber-iq-register.hpp"

JabberIqRegister::JabberIqRegister(const std::string &username, const std::string &password, const std::string &email) {
  Namespace("jabber:iq:register");
  Type("set");
  m_username = new std::string(username);
  m_password = new std::string(password);
  if ("" != email) {
    m_email = new std::string(email);
  }
  else {
    m_email = NULL;
  }
}


JabberIqRegister::~JabberIqRegister(void) {
  delete m_username;
  delete m_password;
  delete m_email;
}


const std::string *JabberIqRegister::render(const std::string *id) const {
  std::ostringstream body;

  body << "<username>" << *m_username << "</username>";
  body << "<password>" << *m_password << "</password>";
  if (NULL != m_email) {
    body << "<email>" << *m_email << "</email>";
  }
  return renderIqStanza(id, Namespace(), body.str());
}


Stanza *JabberIqRegister::parse(const JabberElementNode *query) {
  Stanza *result = NULL;
  const std::string *username = NULL;
  const std::string *password = NULL;
  const std::string *email = NULL;

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
      if ("email" == node->m_name) {
	jabberNodeList_t::const_iterator j=node->m_children.begin();
	const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
	if (NULL != text) {
	  email = new std::string(text->m_data);
	}
      }
    }
  }
  if ((NULL != username) && (NULL != password) && (NULL != email)) {
    result = new JabberIqRegister(*username, *password, *email);
  }
  else {
    // TODO -- decide how, if at all, I handle this error
  }
  return result;
}

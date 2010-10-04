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
#if !defined(_JABBER_IQ_REGISTER_HPP_INCLUDED_)
#define _JABBER_IQ_REGISTER_HPP_INCLUDED_

#include "iq-stanza.hpp"

class JabberIqRegister : public IqStanza {
private:
  std::string *m_username;
  std::string *m_password;
  std::string *m_email;
  // const std::string *render(const std::string &id, const std::string &body) const;

public:
  JabberIqRegister(const std::string &username, const std::string &password, const std::string &email);
  virtual ~JabberIqRegister(void);

  virtual const std::string *render(const std::string *id) const;
  static Stanza *parse(const JabberElementNode *query);
};

#endif // !defined(_JABBER_IQ_REGISTER_HPP_INCLUDED_)

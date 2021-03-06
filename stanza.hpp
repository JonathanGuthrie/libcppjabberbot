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
#if !defined(_STANZA_HPP_INCLUDED_)
#define _STANZA_HPP_INCLUDED_

#include <string>

#include "jabbernode.hpp"

class Stanza {
private:
  const std::string *m_to;
  const std::string *m_from;
  const std::string *m_type;
  const std::string *m_id;
  const std::string *m_namespace;
  const std::string *m_errorMessage;
  int m_error;

public:
  Stanza(void);
  virtual ~Stanza(void);

  void To(const std::string *to);
  void To(const std::string &to);
  void From(const std::string *from);
  void From(const std::string &from);
  void Type(const std::string *type);
  void Type(const std::string &type);
  void Id(const std::string *id);
  void Id(const std::string &id);
  void Namespace(const std::string *name_space);
  void Namespace(const std::string &name_space);
  void ErrorMessage(const std::string *errorMessage);
  void ErrorMessage(const std::string &errorMessage);
  void Error(int error);

  const std::string *To(void) const;
  const std::string *From(void) const;
  const std::string *Type(void) const;
  const std::string *Id(void) const;
  const std::string *Namespace(void) const;
  const std::string *ErrorMessage(void) const;
  // 200 is success
  // 401 means invalid username or password
  // 409 means "Username Not Available"
  int Error(void) const;

  virtual const std::string *render(const std::string *id) const = 0;
  static Stanza *parse(const JabberElementNode *root);

protected:
  const std::string *renderStanza(const std::string *id, const std::string &tag, const std::string &body) const;
};

#endif // !defined(_STANZA_HPP_INCLUDED_)

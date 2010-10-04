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
#if !defined(_MESSAGE_STANZA_HPP_INCLUDED_)
#define _MESSAGE_STANZA_HPP_INCLUDED_

#include "stanza.hpp"

class MessageStanza : public Stanza {
public:
  typedef enum {Normal, Chat, Groupchat, Headline, Error, UnknownMessage} MessageTypes;

private:
  MessageTypes m_type;
  const std::string *m_body;
  const std::string *m_subject;
  const std::string *m_html;
  const std::string *m_thread;
  // TODO -- support the X field

public:
  MessageStanza(void);
  virtual ~MessageStanza(void);

  void Type(MessageTypes type);
  void Body(const std::string *body);
  void Body(const std::string &body);
  void Subject(const std::string *subject);
  void Subject(const std::string &subject);
  void Html(const std::string *html);
  void Html(const std::string &html);
  void Thread(const std::string *thread);
  void Thread(const std::string &thread);
  // TODO -- support the X field

  MessageTypes Type(void) const;
  const std::string *Body(void) const;
  const std::string *Subject(void) const;
  const std::string *Html(void) const;
  const std::string *Thread(void) const;
 
  virtual const std::string *render(const std::string *id) const;
  static Stanza *parse(const JabberElementNode *root);
};

#endif // !defined(_MESSAGE_STANZA_HPP_INCLUDED_)

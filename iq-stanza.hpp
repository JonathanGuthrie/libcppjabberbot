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

#if !defined(_IQ_STANZA_HPP_INCLUDED_)
#define _IQ_STANZA_HPP_INCLUDED_

#include "stanza.hpp"

typedef Stanza *(*stanzaParser)(const JabberElementNode *query);
typedef std::map<std::string, stanzaParser> parseMap_t;

class IqStanza : public Stanza {
private:
  static parseMap_t *m_namespaceParsers;

public:
  IqStanza(void);
  virtual ~IqStanza(void);

  virtual const std::string *render(const std::string *id) const;
  static Stanza *parse(const JabberElementNode *root);

protected:
  const std::string *renderIqStanza(const std::string *id, const std::string *name_space, const std::string &body) const;
};

#endif // !defined(_IQ_STANZA_HPP_INCLUDED_)

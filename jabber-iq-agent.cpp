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

#include "jabber-iq-agent.hpp"

JabberIqAgent::JabberIqAgent(void) {
  Namespace("jabber:iq:agent");
}


JabberIqAgent::~JabberIqAgent(void) {
}


const std::string *JabberIqAgent::render(const std::string *id) const {
  std::ostringstream body;

  return renderIqStanza(id, Namespace(), body.str());
}


Stanza *JabberIqAgent::parse(const JabberElementNode *query) {
  for (jabberNodeList_t::const_iterator i=query->m_children.begin(); i != query->m_children.end(); ++i) {
    const JabberElementNode *node = dynamic_cast<JabberElementNode *>(*i);
    if (NULL != node) {
    }
  }
  return new JabberIqAgent();
}

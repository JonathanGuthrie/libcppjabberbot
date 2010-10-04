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
#if !defined(_JABBERNODE_HPP_INCLUDED_)
#define _JABBERNODE_HPP_INCLUDED_
#include <libxml++/libxml++.h>

class JabberElementNode;

class JabberNode {
public:
  JabberElementNode *m_parent;
protected:
  JabberNode(JabberElementNode *parent);
  virtual ~JabberNode();
};

typedef std::list<JabberNode *> jabberNodeList_t;

class JabberElementNode : public JabberNode {
public:
  const Glib::ustring m_name;
  const xmlpp::SaxParser::AttributeList m_attributes;
  jabberNodeList_t m_children;
  JabberElementNode(JabberElementNode *parent, const Glib::ustring &name, const xmlpp::SaxParser::AttributeList &attributes);
  virtual ~JabberElementNode();
};


class JabberTextNode : public JabberNode {
public:
  const Glib::ustring m_data;
  JabberTextNode(JabberElementNode *parent, const Glib::ustring nodeData);
  virtual ~JabberTextNode();
};


class JabberCommentNode : public JabberNode {
public:
  const Glib::ustring m_data;
  JabberCommentNode(JabberElementNode *parent, const Glib::ustring nodeData);
  virtual ~JabberCommentNode();
};


#endif // !defined(_JABBERNODE_HPP_INCLUDED_)

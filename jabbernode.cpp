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
#include "jabbernode.hpp"

JabberNode::JabberNode(JabberElementNode *parent) : m_parent(parent) {
}

JabberNode::~JabberNode() {}

JabberElementNode::JabberElementNode(JabberElementNode *parent, const Glib::ustring &name, const xmlpp::SaxParser::AttributeList &attributes) : JabberNode(parent), m_name(name), m_attributes(attributes) {
}

JabberElementNode::~JabberElementNode() {
  // SYZYGY -- need a destructor here
}

JabberTextNode::JabberTextNode(JabberElementNode *parent, const Glib::ustring nodeData) : JabberNode(parent), m_data(nodeData) {
}

JabberTextNode::~JabberTextNode() {
  // SYZYGY -- need a destructor here
}

JabberCommentNode::JabberCommentNode(JabberElementNode *parent, const Glib::ustring nodeData) : JabberNode(parent), m_data(nodeData) {
}

JabberCommentNode::~JabberCommentNode() {
  // SYZYGY -- need a destructor here
}

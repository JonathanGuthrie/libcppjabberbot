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

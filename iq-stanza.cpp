#include <sstream>
#include <iostream>

#include "iq-stanza.hpp"
#include "jabber-iq-auth.hpp"
#include "jabber-iq-register.hpp"

IqStanza::IqStanza(void) {}


IqStanza::~IqStanza(void) {}

const std::string *IqStanza::renderIqStanza(const std::string *id, const std::string &name_space, const std::string &body) const {
  std::ostringstream query;

  query << "<query xmlns='" << name_space << "'>" << body << "</query>";
  return renderStanza(id, "iq", query.str());
}

const std::string *IqStanza::render(const std::string *id) const {
  return renderStanza(id, "iq", "");
}


Stanza *IqStanza::parse(const JabberElementNode *root) {
  // If there are no children, then it's an iq stanza
  Stanza *result;
  if (0 == root->m_children.size()) {
    result = new IqStanza();
  }
  else {
    const JabberElementNode *query_node = NULL;
    const JabberElementNode *error_node = NULL;
    const std::string *name_space = NULL;
    for (jabberNodeList_t::const_iterator i=root->m_children.begin(); i != root->m_children.end(); ++i) {
      const JabberElementNode *node = dynamic_cast<JabberElementNode *>(*i);
      if ((NULL != node) && ("query" == node->m_name)) {
	query_node = node;
	for(xmlpp::SaxParser::AttributeList::const_iterator j = node->m_attributes.begin(); j != node->m_attributes.end(); ++j) {
	  if ("xmlns" == j->name) {
	    name_space = new std::string(j->name);
	  }
	}
      }
      if ((NULL != node) && ("error" == node->m_name)) {
	error_node = node;
      }
    }
    if (NULL != name_space) {
      // SYZYGY -- I need to do some sort of namespace map here
      result = JabberIqAuth::parse(query_node, error_node);
    }
  }
  if (NULL != result) {
    for(xmlpp::SaxParser::AttributeList::const_iterator i = root->m_attributes.begin(); i != root->m_attributes.end(); ++i) {
      if ("type" == i->name) {
	result->Type(i->value);
      }
      if ("id" == i->name) {
	result->Id(i->value);
      }
    }
  }
  return result;
}

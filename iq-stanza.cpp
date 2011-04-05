#include <sstream>
// #include <iostream>
#include <cstdlib>

#include "iq-stanza.hpp"
#include "jabber-iq-auth.hpp"
#include "jabber-iq-register.hpp"
#include "jabber-iq-roster.hpp"

parseMap_t *IqStanza::m_namespaceParsers = NULL;

IqStanza::IqStanza(void) {
  if (NULL == m_namespaceParsers) {
    m_namespaceParsers = new parseMap_t;
    
    m_namespaceParsers->insert(parseMap_t::value_type("jabber:iq:auth", JabberIqAuth::parse));
    m_namespaceParsers->insert(parseMap_t::value_type("jabber:iq:register", JabberIqRegister::parse));
    m_namespaceParsers->insert(parseMap_t::value_type("jabber:iq:roster", JabberIqRoster::parse));
  }
}


IqStanza::~IqStanza(void) {}

const std::string *IqStanza::renderIqStanza(const std::string *id, const std::string *name_space, const std::string &body) const {
  std::ostringstream query;

  query << "<query xmlns='" << *name_space << "'>" << body << "</query>";
  return renderStanza(id, "iq", query.str());
}


const std::string *IqStanza::render(const std::string *id) const {
  return renderStanza(id, "iq", "");
}


Stanza *IqStanza::parse(const JabberElementNode *root) {
  // If there are no children, then it's an iq stanza
  Stanza *result;
  std::string errorMessage;
  int error = 0;

  if (0 == root->m_children.size()) {
    result = new IqStanza();
  }
  else {
    const JabberElementNode *query_node = NULL;
    const std::string *name_space = NULL;
    for (jabberNodeList_t::const_iterator i=root->m_children.begin(); i != root->m_children.end(); ++i) {
      const JabberElementNode *node = dynamic_cast<JabberElementNode *>(*i);
      if ((NULL != node) && ("query" == node->m_name)) {
	query_node = node;
	for(xmlpp::SaxParser::AttributeList::const_iterator j = node->m_attributes.begin(); j != node->m_attributes.end(); ++j) {
	  if ("xmlns" == j->name) {
	    name_space = new std::string(j->value);
	  }
	}
      }
      if ((NULL != node) && ("error" == node->m_name)) {
	jabberNodeList_t::const_iterator j=node->m_children.begin();
	const JabberTextNode *text = dynamic_cast<JabberTextNode *>(*j);
	if (NULL != text) {
	  errorMessage = text->m_data;
	}
	
	for(xmlpp::SaxParser::AttributeList::const_iterator k = node->m_attributes.begin(); k != node->m_attributes.end(); ++k) {
	  if ("code" == k->name) {
	    error = strtod(k->value.c_str(), NULL);
	  }
	}
      }
    }
    if (NULL != name_space) {
      parseMap_t::iterator parser = m_namespaceParsers->find(*name_space);
      if (parser != m_namespaceParsers->end()) {
	result = (*parser->second)(query_node);
      }
      else {
	// std::cout << "I don't recognize the name space " << *name_space << std::endl;
	// SYZYGY -- log, I guess
	// I don't know the name space.  Log, I guess, and return a plain stanza
	result = new IqStanza();
      }
    }
    else {
      result = new IqStanza();
    }
  }
  if (NULL != result) {
    if ("" != errorMessage) {
      result->ErrorMessage(errorMessage);
      result->Error(error);
    }
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

#include <sstream>

#include "jabber-iq-roster.hpp"

JabberIqRoster::JabberIqRoster(const std::string &type) {
  Namespace("jabber:iq:roster");
  Type(type);
}


JabberIqRoster::~JabberIqRoster(void) {
}


const std::string *JabberIqRoster::render(const std::string *id) const {
  return renderIqStanza(id, Namespace(), "");
}


Stanza *JabberIqRoster::parse(const JabberElementNode *query) {
  return new JabberIqRoster();
}

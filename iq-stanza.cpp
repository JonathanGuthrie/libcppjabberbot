#include <sstream>

#include "iq-stanza.hpp"

IqStanza::IqStanza(void) {}


IqStanza::~IqStanza(void) {}

const std::string *IqStanza::renderIqStanza(const std::string &id, const std::string &name_space, const std::string &body) const {
  std::ostringstream query;

  query << "<query xmlns='" << name_space << "'>" << body << "</query>";
  return renderStanza(id, "iq", query.str());
}

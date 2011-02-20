#include <sstream>
// #include <iostream>
#include <cstdlib>

#include "features-stanza.hpp"

FeaturesStanza::FeaturesStanza(void) {
}


FeaturesStanza::~FeaturesStanza(void) {}

const std::string *FeaturesStanza::renderFeaturesStanza(const std::string *id, const std::string *name_space, const std::string &body) const {
  return renderStanza(id, "stream:features", "");
}


const std::string *FeaturesStanza::render(const std::string *id) const {
  return renderStanza(id, "stream:features", "");
}


Stanza *FeaturesStanza::parse(const JabberElementNode *root) {
  return new FeaturesStanza();
}

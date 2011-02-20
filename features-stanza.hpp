#if !defined(_FEATURES_STANZA_HPP_INCLUDED_)
#define _FEATURES_STANZA_HPP_INCLUDED_

#include "stanza.hpp"

class FeaturesStanza : public Stanza {
private:

public:
  FeaturesStanza(void);
  virtual ~FeaturesStanza(void);

  virtual const std::string *render(const std::string *id) const;
  static Stanza *parse(const JabberElementNode *root);

protected:
  const std::string *renderFeaturesStanza(const std::string *id, const std::string *name_space, const std::string &body) const;
};

#endif // !defined(_FEATURES_STANZA_HPP_INCLUDED_)

#if !defined(_JABBER_IQ_ROSTER_HPP_INCLUDED_)
#define _JABBER_IQ_ROSTER_HPP_INCLUDED_

#include "iq-stanza.hpp"

class JabberIqRoster : public IqStanza {
private:

public:
  JabberIqRoster(const std::string &type = "get");
  virtual ~JabberIqRoster(void);

  virtual const std::string *render(const std::string *id) const;
  static Stanza *parse(const JabberElementNode *query);
};

#endif // !defined(_JABBER_IQ_ROSTER_HPP_INCLUDED_)

#if !defined(_PRESENCE_STANZA_HPP_INCLUDED_)
#define _PRESENCE_STANZA_HPP_INCLUDED_

#include "stanza.hpp"

class PresenceStanza : public Stanza {
public:
  typedef enum {Available, Unavailable, Probe, Subscribe, Unsubscribe, Subscribed, Unsubscribed} PresenceTypes;

private:
  PresenceTypes m_type;
  const std::string *m_show;
  const std::string *m_status;
  int m_priority;

public:
  PresenceStanza(void);
  virtual ~PresenceStanza(void);

  void Type(PresenceTypes type);
  void Show(const std::string *show);
  void Show(const std::string &show);
  void Status(const std::string *status);
  void Status(const std::string &status);
  void Priority(int priority);  // less than 1 disables priority
  // TODO -- support the X field

  PresenceTypes Type(void) const;
  const std::string *Show(void) const;
  const std::string *Status(void) const;
  int Priority(void) const;

  virtual const std::string *render(const std::string *id) const;
  static Stanza *parse(const JabberElementNode *root);
};

#endif // !defined(_PRESENCE_STANZA_HPP_INCLUDED_)

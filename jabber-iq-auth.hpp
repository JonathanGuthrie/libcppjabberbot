#if !defined(_JABBER_IQ_AUTH_HPP_INCLUDED_)
#define _JABBER_IQ_AUTH_HPP_INCLUDED_

#include "iq-stanza.hpp"

class JabberIqAuth : public IqStanza {
private:
  std::string *m_username;
  std::string *m_password;
  std::string *m_resource;
  const std::string *render(const std::string &id, const std::string &body) const;

public:
  JabberIqAuth(const std::string &username, const std::string &password, const std::string &resource);
  virtual ~JabberIqAuth(void);

  virtual const std::string *render(const std::string &id) const;
};

#endif // !defined(_JABBER_IQ_AUTH_HPP_INCLUDED_)

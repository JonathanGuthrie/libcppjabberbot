#if !defined(_IQ_STANZA_HPP_INCLUDED_)
#define _IQ_STANZA_HPP_INCLUDED_

#include "stanza.hpp"

class IqStanza : public Stanza {
private:
  std::string *m_password;

public:
  IqStanza(void);
  virtual ~IqStanza(void);

  virtual const std::string *render(const std::string *id) const = 0;

protected:
  const std::string *renderIqStanza(const std::string *id, const std::string &name_space, const std::string &body) const;
};

#endif // !defined(_IQ_STANZA_HPP_INCLUDED_)

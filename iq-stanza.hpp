#if !defined(_IQ_STANZA_HPP_INCLUDED_)
#define _IQ_STANZA_HPP_INCLUDED_

#include "stanza.hpp"

typedef Stanza *(*stanzaParser)(const JabberElementNode *query, const JabberElementNode *error);
typedef std::map<std::string, stanzaParser> parseMap_t;

class IqStanza : public Stanza {
private:
  std::string *m_password;
  static parseMap_t *m_namespaceParsers;

public:
  IqStanza(void);
  virtual ~IqStanza(void);

  virtual const std::string *render(const std::string *id) const;
  static Stanza *parse(const JabberElementNode *root);

protected:
  const std::string *renderIqStanza(const std::string *id, const std::string &name_space, const std::string &body) const;
};

#endif // !defined(_IQ_STANZA_HPP_INCLUDED_)

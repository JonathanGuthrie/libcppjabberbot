#if !defined(_STANZA_HPP_INCLUDED_)
#define _STANZA_HPP_INCLUDED_

#include <string>

class Stanza {
private:
  std::string *m_to;
  std::string *m_from;
  std::string *m_type;
  std::string *m_id;

public:
  Stanza(void);
  virtual ~Stanza(void);

  void To(const std::string &to);
  void From(const std::string &from);
  void Type(const std::string &type);
  void Id(const std::string &id);

  const std::string &To(void) const;
  const std::string &From(void) const;
  const std::string &Type(void) const;
  const std::string &Id(void) const;

  virtual const std::string *render(const std::string &id) const = 0;

protected:
  const std::string *renderStanza(const std::string &id, const std::string &tag, const std::string &body) const;
};

#endif // !defined(_STANZA_HPP_INCLUDED_)

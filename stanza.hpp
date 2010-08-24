#if !defined(_STANZA_HPP_INCLUDED_)
#define _STANZA_HPP_INCLUDED_

#include <string>

#include "jabbernode.hpp"

class Stanza {
private:
  std::string *m_to;
  std::string *m_from;
  std::string *m_type;
  std::string *m_id;
  std::string *m_namespace;
  std::string *m_errorMessage;
  int m_error;

public:
  Stanza(void);
  virtual ~Stanza(void);

  void To(const std::string &to);
  void From(const std::string &from);
  void Type(const std::string &type);
  void Id(const std::string &id);
  void Namespace(const std::string &name_space);
  void ErrorMessage(const std::string &errorMessage);
  void Error(int error);

  const std::string &To(void) const;
  const std::string &From(void) const;
  const std::string &Type(void) const;
  const std::string &Id(void) const;
  const std::string &Namespace(void) const;
  const std::string &ErrorMessage(void) const;
  // 200 is success
  // 401 means invalid username or password
  // 409 means "Username Not Available"
  int Error(void) const;

  virtual const std::string *render(const std::string *id) const = 0;
  static Stanza *parse(const JabberElementNode *root);

protected:
  const std::string *renderStanza(const std::string *id, const std::string &tag, const std::string &body) const;
};

#endif // !defined(_STANZA_HPP_INCLUDED_)

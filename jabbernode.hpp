#if !defined(_JABBERNODE_HPP_INCLUDED_)
#define _JABBERNODE_HPP_INCLUDED_
#include <libxml++/libxml++.h>

class JabberElementNode;

class JabberNode {
public:
  JabberElementNode *m_parent;
protected:
  JabberNode(JabberElementNode *parent);
  virtual ~JabberNode();
};

typedef std::list<JabberNode *> jabberNodeList_t;

class JabberElementNode : public JabberNode {
public:
  const Glib::ustring m_name;
  const xmlpp::SaxParser::AttributeList m_attributes;
  jabberNodeList_t m_children;
  JabberElementNode(JabberElementNode *parent, const Glib::ustring &name, const xmlpp::SaxParser::AttributeList &attributes);
  virtual ~JabberElementNode();
};


class JabberTextNode : public JabberNode {
public:
  const Glib::ustring m_data;
  JabberTextNode(JabberElementNode *parent, const Glib::ustring nodeData);
  virtual ~JabberTextNode();
};


class JabberCommentNode : public JabberNode {
public:
  const Glib::ustring m_data;
  JabberCommentNode(JabberElementNode *parent, const Glib::ustring nodeData);
  virtual ~JabberCommentNode();
};


#endif // !defined(_JABBERNODE_HPP_INCLUDED_)

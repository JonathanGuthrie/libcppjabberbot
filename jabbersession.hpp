#if !defined(_JABBERSESSION_HPP_INCLUDED_)
#define _JABBERSESSION_HPP_INCLUDED_

#include <map>
#include <list>

#include "socket.hpp"
#include "stanza.hpp"
#include "jabbernode.hpp"

class JabberSession;

typedef void (*handler_t)(const Stanza &request, class JabberSession *session);
typedef std::map<std::string, handler_t> handlerMap_t;

class JabberSession : public xmlpp::SaxParser {
private:
  typedef enum { Connected, GotStreamTag, Closing, Closed, Error } jabberSessionState_t; 
  typedef struct {Stanza *s; pthread_mutex_t c;} jabberEvent_t;
  typedef std::map<Glib::ustring, jabberEvent_t *> jabberEventMap_t;

  // virtual void on_start_document();
  // virtual void on_end_document();
  virtual void on_start_element(const Glib::ustring &name, const AttributeList &attributes);
  virtual void on_end_element(const Glib::ustring &name);
  virtual void on_characters(const Glib::ustring &characters);
  virtual void on_comment(const Glib::ustring &text);
  virtual void on_warning(const Glib::ustring &text);
  virtual void on_error(const Glib::ustring &text);
  virtual void on_fatal_error(const Glib::ustring &text);

  pthread_t m_listenerThread;
  pthread_mutex_t m_stateMutex;
  jabberSessionState_t m_state;
  static void *ListenerThreadFunction(void *);
  Socket m_s;
  int m_depth;
  Glib::ustring m_sessionIdentifier;
  Glib::ustring m_logicalServer;
  jabberEventMap_t m_jabberEvents;
  unsigned long m_idCount;
  JabberElementNode *m_node;
  handlerMap_t m_handlers;

public:
  JabberSession(const std::string &host, unsigned short port, bool isSecure);
  virtual ~JabberSession(void);

  const Stanza *SendMessage(const Stanza &request, bool expectingReply);
  void Register(handler_t handler, const std::string &name_space);
};


#endif // _JABBERSESSION_HPP_INCLUDED_

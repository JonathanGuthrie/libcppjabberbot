/*
 * Copyright 2010 Jonathan R. Guthrie
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(_JABBERSESSION_HPP_INCLUDED_)
#define _JABBERSESSION_HPP_INCLUDED_

#include <map>
#include <list>

#include "socket.hpp"
#include "stanza.hpp"
#include "jabbernode.hpp"

class JabberSession;
class MessageStanza;
class PresenceStanza;
class IqStanza;

/**
 * \brief The handler types
 *
 * The handlers are all passed a context, which is whatever was passed to the register function.  The intent is
 * to use it to pass a class pointer so that the handler function could be a static method.
 *
 * Each handler is passed the stanza that triggered the call to the handler.  This stanza is different for 
 * different handler types.
 *
 * The session pointer is the "this" pointer for whatever session invoked the handler.  The intent is for
 * a simple handler to be able to use the session value to send XMPP stanzas.
 * 
 * Each handler is defined to return an int, which is currently ignored.  I recommend you return a 0 from
 * handler functions until the meaning of other values is defined.
 *
 * Since an IQ stanza handler depends on the namespace, I also define a map between the namespace and the particular
 * handler.  IQ handlers are specified for each namespace, while message and presence handlers are specified only
 * for the stanza type.
 */
typedef int (*messageHandler_t)(void *context, const MessageStanza &request, class JabberSession *session);
typedef int (*presenceHandler_t)(void *context, const PresenceStanza &request, class JabberSession *session);
typedef int (*iqHandler_t)(void *context, const IqStanza &request, class JabberSession *session);
typedef std::map<std::string, iqHandler_t> iqHandlerMap_t;

/**
 * \brief The JabberSession class
 *
 * The JabberSession class contains the state for a given connection to an Jabber server.  A
 * Jabber/XMPP connection is serialized by the fact that it's XML.  This class is derived from the
 * GNOME SAX parser because one fundamental job of this class is to carve the XML stream up
 * into stanzas.  The other is to package stanzas up for transmission.
 */
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
  iqHandlerMap_t m_iqHandlers;
  void *m_iqContext;
  presenceHandler_t m_presenceHandler;
  void *m_presenceContext;
  messageHandler_t m_messageHandler;
  void *m_messageContext;

public:
  /**
   * \brief The Constructor
   *
   * Since items in this class represent connections to Jabber/XMPP servers, the required options
   * are the hostname (or IP address, encoded as a string) and port used to connect to the server and
   * a boolean flag that determines whether an SSL connection is negotiated upon connection to the server.
   * The optional parameter is the logical name for the server to be presented to the server in the 
   * initial XML stanza.
   *
   * The SSL flag is only for the old Jabber-style connection.  If the connection is the new XMPP-Style connection,
   * then this flag should be set to FALSE and the SSL negotiation process should be completed at the appropriate
   * time.
   *
   * If the optional parameter is not present, then the logical server name is derived from the host string.  In
   * that case, or if it's set to an explicit NULL value, then the host name MUST NOT be an IP address, but MUST
   * be a resolvable DNS name which needs to match a logical name the server recognizes as one it supports.
   *
   * The constructor does the usual member initialization stuff, including establishing a connection to the server
   * and creating a listener thread.  The constructor does not return until the outermost stream tag is received
   * from the Jabber/XMPP server
   *
   * @param host a constant string reference, which should contain a host name or IP address
   * @param 
   */
  JabberSession(const std::string &host, unsigned short port, bool isSecure, const std::string *jabberServerName = NULL);

  /**
   * \brief The Destructor
   *
   * The destructor sends the stream end tag and then cleanly shuts down the listener thread.
   */
  virtual ~JabberSession(void);

  /**
   * \brief Send an XMPP stanza
   *
   * @param request a constant reference to a stanza object that is the message to be sent
   * @param expectingReply a boolean that is true if the client is expecting a reply, and false otherwise.
   * @see JabberSession()
   * @see ~JabberSession()
   * @see Register
   * @return A pointer to the stanza that was returned in response, or NULL if no replyreturn was expected.
   */
  const Stanza *SendMessage(const Stanza &request, bool expectingReply);
  void Register(void *context, iqHandler_t handler, const std::string &name_space);
  void Register(void *context, presenceHandler_t handler);
  void Register(void *context, messageHandler_t handler);
};


#endif // _JABBERSESSION_HPP_INCLUDED_

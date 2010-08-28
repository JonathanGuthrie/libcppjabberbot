#include <iostream>

#include "jabbersession.hpp"
#include "jabber-iq-auth.hpp"
#include "jabber-iq-register.hpp"
#include "jabber-iq-roster.hpp"
#include "presence-stanza.hpp"


int presence_notification(const PresenceStanza &request, class JabberSession *session) {
  std::cout << "It got into the presence_notification handler" << std::endl;
  if (PresenceStanza::Subscribe == request.Type()) {
    PresenceStanza response;
    response.Type(PresenceStanza::Subscribed);
    response.To(request.From());
    session->SendMessage(response, false);

    response.Type(PresenceStanza::Subscribe);
    session->SendMessage(response, false);
  }
  if (PresenceStanza::Unsubscribe == request.Type()) {
    PresenceStanza response;
    response.Type(PresenceStanza::Unsubscribed);
    response.To(request.From());
    session->SendMessage(response, false);

    response.Type(PresenceStanza::Unsubscribe);
    session->SendMessage(response, false);
  }
  return 0;
}

int roster_notification(const IqStanza &request, class JabberSession *session)  {
  std::cout << "It got into the roster_notification handler" << std::endl;
  return 0;
}

int main(void) {
  JabberSession session("jabber.brokersys.com", 5222, false);

  session.Register(presence_notification);
  session.Register(roster_notification, "jabber:iq:roster");
  JabberIqAuth login_stanza("jabber-bot", "zu2ooHah", "TestBot");
  const Stanza *response= session.SendMessage(login_stanza, true);
  // std::cout << "The first login response is " << *(response->render(NULL)) << std::endl;
  if (200 != response->Error()) {
    std::cout << "The first login response is " << response->ErrorMessage() << std::endl;
    JabberIqRegister register_stanza("jabber-bot", "zu2ooHah", "jguthrie@brokersys.com");
    response = session.SendMessage(register_stanza, true);
    // std::cout << "The register response is " << *(response->render(NULL)) << std::endl;
    if (200 != response->Error()) {
      std::cout << "The register response is " << response->ErrorMessage() << std::endl;
    }
    else {
      JabberIqAuth login2_stanza("jabber-bot", "zu2ooHah", "TestBot");
      response = session.SendMessage(login2_stanza, true);
      // std::cout << "The second login response is " << *(response->render(NULL)) << std::endl;
      if (200 != response->Error()) {
	std::cout << "The second login failed with a message of " << response->ErrorMessage() << std::endl;

      }
    }
  }
  JabberIqRoster rosterQuery;
  session.SendMessage(rosterQuery, false);

  delete response;

  sleep(1);
  PresenceStanza presenceNotification;
  presenceNotification.Type(PresenceStanza::Available);
  presenceNotification.Status(NULL);
  presenceNotification.Priority(-1);
  session.SendMessage(presenceNotification, false);

  sleep(20);

  presenceNotification.Type(PresenceStanza::Unavailable);
  presenceNotification.Status(NULL);
  presenceNotification.Priority(-1);
  session.SendMessage(presenceNotification, false);

  return 0;
}

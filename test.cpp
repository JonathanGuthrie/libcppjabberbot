#include <iostream>

#include "jabbersession.hpp"
#include "jabber-iq-auth.hpp"
#include "jabber-iq-register.hpp"
#include "jabber-iq-roster.hpp"
#include "presence-stanza.hpp"

void presence_notification(const Stanza &request, class JabberSession *session)  {
  std::cout << "It got into the presence_notification handler" << std::endl;
}

int main(void) {
  JabberSession session("jabber.brokersys.com", 5222, false);

  session.Register(presence_notification, "jabber:iq:roster");
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

  PresenceStanza presenceNotification;
  presenceNotification.Type(PresenceStanza::Available);
  presenceNotification.Show("I am Here");
  presenceNotification.Status("normal");
  presenceNotification.Priority(255);
  session.SendMessage(presenceNotification, false);

  presenceNotification.Type(PresenceStanza::Subscribe);
  presenceNotification.Show(NULL);
  presenceNotification.Status(NULL);
  presenceNotification.Priority(-1);
  presenceNotification.To("cybersmythe@jabber.brokersys.com");
  session.SendMessage(presenceNotification, false);

  sleep(100);

  return 0;
}

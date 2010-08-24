#include <iostream>

#include "jabbersession.hpp"
#include "jabber-iq-auth.hpp"
#include "jabber-iq-register.hpp"

int main(void) {
  JabberSession session("jabber.brokersys.com", 5222, false);

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
  delete response;

  sleep(1);

  return 0;
}

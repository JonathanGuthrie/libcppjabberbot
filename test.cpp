#include <iostream>

#include "jabbersession.hpp"
#include "jabber-iq-auth.hpp"
#include "jabber-iq-register.hpp"

int main(void) {
  JabberSession session("jabber.brokersys.com", 5222, false);

  JabberIqAuth login_stanza("jabber-bot", "XXXzu2ooHah", "TestBot");
  const Stanza *response= session.SendMessage(login_stanza, true);
  std::cout << "The first login response is " << *(response->render(NULL)) << std::endl;
  JabberIqRegister register_stanza("jabber-bot", "zu2ooHah", "TestBot");
  response = session.SendMessage(register_stanza, true);
  std::cout << "The register response is " << *(response->render(NULL)) << std::endl;
  JabberIqAuth login2_stanza("jabber-bot", "zu2ooHah", "TestBot");
  response = session.SendMessage(login2_stanza, true);
  std::cout << "The second login response is " << *(response->render(NULL)) << std::endl;
  delete response;

  sleep(1);

  return 0;
}

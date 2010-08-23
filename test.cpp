#include <iostream>

#include "jabbersession.hpp"
#include "jabber-iq-auth.hpp"
#include "jabber-iq-register.hpp"

int main(void) {
  JabberSession session("jabber.brokersys.com", 5222, false);

  // JabberIqRegister register_stanza("jabber-bot", "zu2ooHah", "TestBot");
  // session.SendRequest(register_stanza);
  // JabberIqAuth login_stanza("jabber-bot", "zu2ooHah", "TestBot");
  JabberIqAuth login_stanza("jabber-bot", "XXXzu2ooHah", "TestBot");
  const Stanza *response= session.SendMessage(login_stanza, true);
  std::cout << "The response is " << *(response->render(NULL)) << std::endl;
  delete response;

  sleep(1);

  return 0;
}

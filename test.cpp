#include <iostream>

#include "jabbersession.hpp"
#include "jabber-iq-auth.hpp"
#include "jabber-iq-register.hpp"

int main(void) {
  JabberSession session("jabber.brokersys.com", 5222, false);

  // JabberIqRegister register_stanza("jabber-bot", "zu2ooHah", "TestBot");
  // session.SendRequest(register_stanza);
  JabberIqAuth login_stanza("jabber-bot", "zu2ooHah", "TestBot");
  session.SendMessage(login_stanza, true);

  sleep(1);

  return 0;
}

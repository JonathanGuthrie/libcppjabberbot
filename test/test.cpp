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
#include <iostream>

#include "jabber-bot/jabber-bot.hpp"


int echomessages(void *context, const MessageStanza &request, class JabberSession *session) {
  std::cout << "Received a " << request.Type() << " message from " << *request.From() << std::endl;
  if (NULL != request.Body()) {
    std::cout << "With a body of " << *request.Body() << std::endl;
    MessageStanza response;
    response.Type(request.Type());
    response.To(request.From());
    response.Body(request.Body());
    session->SendMessage(response, false);
  }
  return 0;
}


int presence_notification(void *context, const PresenceStanza &request, class JabberSession *session) {
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

int roster_notification(void *context, const IqStanza &request, class JabberSession *session)  {
  std::cout << "It got into the roster_notification handler" << std::endl;
  return 0;
}

int main(void) {
  JabberSession session("jabber.brokersys.com", 5222, false);

  session.Register(NULL, presence_notification);
  session.Register(NULL, echomessages);
  session.Register(NULL, roster_notification, "jabber:iq:roster");
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
  presenceNotification.Status("I'm just a bot, so sue me.  No, wait.  You can't");
  presenceNotification.Priority(255);
  session.SendMessage(presenceNotification, false);

  sleep(100);

  presenceNotification.Type(PresenceStanza::Unavailable);
  presenceNotification.Status(NULL);
  presenceNotification.Priority(-1);
  session.SendMessage(presenceNotification, false);

  return 0;
}

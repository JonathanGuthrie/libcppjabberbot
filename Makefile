# Copyright 2010 Jonathan R. Guthrie

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

# http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

CC=g++
CXXFLAGS=-Wall -g `pkg-config libxml++-2.6 --cflags` -fPIC
LDFLAGS=-lssl -lexpat `pkg-config libxml++-2.6 --libs` -shared -Wl,-soname,libjabber-bot.so.1

libjabber-bot.so.1.1:  stanza.o jabbernode.o jabbersession.o socket.o \
	iq-stanza.o jabber-iq-agent.o jabber-iq-agents.o jabber-iq-auth.o jabber-iq-register.o jabber-iq-roster.o \
	presence-stanza.o message-stanza.o features-stanza.o
	$(CC) $(LDFLAGS) -o $@ $^

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : Makefile ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

SOURCES=stanza.cpp \
	jabbernode.cpp \
	jabbersession.cpp \
	socket.cpp \
	iq-stanza.cpp \
	jabber-iq-agent.cpp \
	jabber-iq-agents.cpp \
	jabber-iq-auth.cpp \
	jabber-iq-register.cpp \
	jabber-iq-roster.cpp \
	presence-stanza.cpp \
	message-stanza.cpp \
	features-stanza.cpp

include $(SOURCES:.cpp=.d)

clean:
	rm -f *.o *.d libjabber-bot.so libjabber-bot.so.1 libjabber-bot.so.1.1


CC=g++
CXXFLAGS=-Wall -g `pkg-config libxml++-2.6 --cflags` -fPIC
LDFLAGS=-lssl -lexpat `pkg-config libxml++-2.6 --libs` -shared -Wl,-soname,libjabber-bot.so.1

libjabber-bot.so: libjabber-bot.so.1.1 libjabber-bot.so.1
	ln -s libjabber-bot.so.1.1 libjabber-bot.so

libjabber-bot.so.1: libjabber-bot.so.1.1
	ln -s libjabber-bot.so.1.1 libjabber-bot.so.1

libjabber-bot.so.1.1:  stanza.o jabbernode.o jabbersession.o socket.o \
	iq-stanza.o jabber-iq-auth.o jabber-iq-register.o jabber-iq-roster.o \
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
	jabber-iq-auth.cpp \
	jabber-iq-register.cpp \
	jabber-iq-roster.cpp \
	presence-stanza.cpp \
	message-stanza.cpp \
	features-stanza.cpp

include $(SOURCES:.cpp=.d)

clean:
	rm -f *.o *.d libjabber-bot.so libjabber-bot.so.1 libjabber-bot.so.1.1


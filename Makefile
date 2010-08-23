CC=g++
CXXFLAGS=-Wall -g `pkg-config libxml++-2.6 --cflags`
LDFLAGS=-lssl -lexpat `pkg-config libxml++-2.6 --libs`

test: test.o stanza.o jabbernode.o jabbersession.o socket.o iq-stanza.o jabber-iq-auth.o jabber-iq-register.o

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
	test.cpp

include $(SOURCES:.cpp=.d)

clean:
	rm -f *.o *.d


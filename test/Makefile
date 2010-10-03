CC=g++
CXXFLAGS=-Wall -g `pkg-config libxml++-2.6 --cflags`
LDFLAGS=-L/usr/local/lib -ljabber-bot

# Run test from this directory with a command like:
#   LD_LIBRARY_PATH=.. ./test
test: test.o

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : Makefile ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

SOURCES=test.cpp

include $(SOURCES:.cpp=.d)

clean:
	rm -f *.o *.d test

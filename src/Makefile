# Thanks to Sam Holden for providing the original version of this Makefile.

CC?=gcc
BIN?=../bin/fbbi
DEBUGFLAGS?=
#DEBUGFLAGS?=-DFBBI_DEBUG -DDEBUG_STACK
CFLAGS?=-ansi -pedantic -g -Wall -O $(DEBUGFLAGS)
OBJECTS= \
	bf98spc.o  \
	f98fp.o    \
	f98i.o     \
	f98ip.o    \
	f98stack.o \
	fbbi.o     \
	fp/NULL.o  \
	fp/ROMA.o  \

all: fbbi

fbbi: $(OBJECTS)
	$(CC) -o$(BIN) $(OBJECTS)

strip: $(BIN)
	strip $(BIN)

clean:
	rm -f *.o
	rm -f fp/*.o

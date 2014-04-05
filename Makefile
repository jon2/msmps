# Generic Makefile
# Set the "SOURCES" and the "EXECUTABLE" variables to whatever is needed.  Recycle often.

CC=gcc
CFLAGS=-g -Wall -c -I.
LDFLAGS=
SOURCES=memps.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=memps

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o memps *~


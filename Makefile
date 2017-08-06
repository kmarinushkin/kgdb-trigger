PROGNAME=kgdb-trigger
VERSION=1.0

Q=@
CC=gcc
CP=cp

CFLAGS=-Wall -Wextra -Werror
DEFINES=-DPROGNAME=\"$(PROGNAME)\" -DVERSION=\"$(VERSION)\"
SOURCES=$(wildcard *.c)
DESTDIR=/usr/bin

all: $(PROGNAME)

$(PROGNAME): $(SOURCES)
	$(Q)$(CC) $(CFLAGS) $(DEFINES) $(SOURCES) -o $(PROGNAME)

install: all
	$(Q)$(CP) $(PROGNAME) $(DESTDIR)/$(PROGNAME)


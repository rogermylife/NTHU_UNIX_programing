ROOT=.
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)/Make.defines.$(PLATFORM)

ifeq "$(PLATFORM)" "linux"
	ZAP = zap
else
	ZAP =
endif


PROGS =	access cdpwd changemod devrdev filetype mycd umask unlink $(ZAP)
MOREPROGS = findLink

#all:	$(PROGS) $(MOREPROGS)

%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

all:	findLink.c $(LIBAPUE)
	$(CC) $(CFLAGS) $(NAMEMAX) findLink.c -o findLink $(LDFLAGS) $(LDLIBS)

official:	ftw8.c $(LIBAPUE)
	$(CC) $(CFLAGS) $(NAMEMAX) official_ftw8.c -o official_ftw8 $(LDFLAGS) $(LDLIBS)

clean:

clean:
	rm -f $(PROGS) $(MOREPROGS) $(TEMPFILES) *.o $(ZAP)

include $(ROOT)/Make.libapue.inc


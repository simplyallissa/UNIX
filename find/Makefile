ROOT=../../csci3751/apue.3e/
INTRO=../../csci3751/apue.3e/intro2/
PLATFORM=$(shell $(ROOT)/systype.sh)
include $(ROOT)Make.defines.$(PLATFORM)

PROGS =	$(INTRO)/getcputc $(INTRO)/hello $(INTRO)/ls1 $(INTRO)/mycat $(INTRO)/shell1 $(INTRO)/shell2 $(INTRO)/testerror $(INTRO)/uidgid myShell2 shell2.c find find.c

all:	$(PROGS)

%:	%.c $(LIBAPUE)
	$(CC) $(CFLAGS) $@.c -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(PROGS) $(TEMPFILES) *.o

include $(ROOT)/Make.libapue.inc

include $(PROJ_ROOT)/Makefile.inc

CFLAGS=$(COMMON_CFLAGS) -I.
LIB_OUT=algolib.a

all:
	rm -rf *.o *.a
	$(CC) $(CFLAGS) *.c
	$(AR) -cvq $(LIB_OUT) *.o
	cp $(LIB_OUT) $(PROJ_ROOT)/libs/$(LIB_OUT)
	rm -rf *.o *.a
clean:
	rm -rf *.o *.a

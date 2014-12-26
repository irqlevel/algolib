include $(PROJ_ROOT)/Makefile.inc
BIN_OUT=algolib_test
CFLAGS=$(COMMON_CFLAGS)
all: algolib_test
	cp algolib_test $(PROJ_ROOT)/bin/algolib_test
	rm -rf *.o algolib_test

algolib_test:	main.o
	$(CC) main.o $(PROJ_ROOT)/libs/algolib.a -o $(BIN_OUT)

main.o:	main.c
	$(CC) $(CFLAGS) main.c

clean:
	rm -rf *.o algolib_test


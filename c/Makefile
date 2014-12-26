export PROJ_ROOT=$(CURDIR)
export ARCH_BITS=$(shell getconf LONG_BIT)
all:
	@echo $(PROJ_ROOT)
	rm -r -f bin/ objs/ libs/
	mkdir bin objs libs
	cd algolib; make
	cd test; make
clean:
	cd algolib; make clean
	cd test; make clean
	rm -r -f bin/ objs/ libs/

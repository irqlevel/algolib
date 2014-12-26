all:
	cd java ; mvn package
	cd c ; make

clean:
	cd c ; make clean

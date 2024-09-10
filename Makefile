CC = g++
#CFLAGS = -Wall -Wextra -O2
LDLIBS = -lm

build: curatare.o fortificatii.o beamdrone.o curse.o regele.o

run-p1:
	./curatare

run-p2:
	./fortificatii

run-p3:
	./beamdrone

run-p4:
	./curse

run-p5:
	./regele


curatare.o: curatare.cpp
	g++ curatare.cpp -o curatare

fortificatii.o: fortificatii.cpp
	g++ fortificatii.cpp -o fortificatii

beamdrone.o: beamdrone.cpp
	g++ beamdrone.cpp -o beamdrone

curse.o: curse.cpp
	g++ curse.cpp -o curse

.PHONY: clean

clean:
	rm -rf *.o curatare fortificatii beamdrone curse
libh.a: hasard.o hasardcpp.o Makefile
	rm -f libh.a
	ar q libh.a hasard.o hasardcpp.o

all: appelf appelc libh.a

appelf: appelf.f hasard.o Makefile
	gfortran appelf.f hasard.o -o appelf 

appelc: appelc.c hasard.o Makefile
	gcc appelc.c hasard.o -o appelc -lm

hasard.o:hasard.c hasard.h Makefile
	gcc  -c hasard.c -Wall -O3

hasardcpp.o:hasardcpp.cpp Makefile
	g++  -c hasardcpp.cpp -Wall -O3

install: libh.a Makefile
	cp -f libh.a ~/lib/
	cp -f hasard.h ~/include

clean:
	rm -f appelc appelf hasard.o hasardcpp.o libh.a
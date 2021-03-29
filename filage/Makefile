libfilage.a:filage.o filagecpp.o  Makefile
	ar q libfilage.a filage.o filagecpp.o

all: writec writecpp writef readc readcpp readf xtractcomments

install: libfilage.a xtractcomments filage.h Makefile
	cp -f libfilage.a ~/lib/
	cp -f filage.h ~/include/
	cp -f xtractcomments ~/bin/

writec: writec.c filage.o Makefile
	gcc writec.c -o writec filage.o -fPIC

readc: readc.c filage.o Makefile
	gcc readc.c -o readc filage.o -fPIC

writecpp: writecpp.cpp filage.o Makefile
	c++  writecpp.cpp -o writecpp filage.o -fPIC

readcpp: readcpp.cpp filage.o Makefile
	c++  readcpp.cpp -o readcpp filage.o -Wwrite-strings -fPIC

writef: writef.F filage.o Makefile
	gfortran-4.6 writef.F -o writef filage.o -g -fbackslash

readf: readf.F filage.o Makefile
	gfortran-4.6 readf.F -o readf filage.o -g -fbackslash

filage.o: filage.c filage.h Makefile
	gcc -c filage.c -Wall -O3 -fPIC

filagecpp.o: filagecpp.cpp filagecpp.h Makefile
	g++ -c filagecpp.cpp -Wall -O3 -fPIC

xtractcomments:xtractcomments.cpp libfilage.a Makefile
	c++ xtractcomments.cpp -o xtractcomments -I. -L. -lfilage -Wall -O3 -fPIC

clean:
	rm -f libfilage.a  readc writecpp readf writec xtractcomments filage.o readcpp writef

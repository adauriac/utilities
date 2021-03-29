CPPFLAGS = -Wall -O3

all: libjc.a install

libjc.a : enreg/libenreg.a filage/libfilage.a alea/libh.a jc.h etc/libetc.a Makefile
	rm -f libjc.a
	ar q libjc.a enreg/libenreg.o filage/filage.o filage/filagecpp.o alea/hasard.o alea/hasardcpp.o etc/libetc.o
	echo "#ifndef _JC" > jc.h
	echo "#define _JC" >> jc.h
	echo "#include <iostream>" >> jc.h
	echo "using namespace std;" >> jc.h
	cat enreg/enreg.h filage/filage.h filage/filagecpp.h alea/hasard.h alea/hasardcpp.h etc/etc.h >> jc.h
	echo "#endif" >> jc.h

install:
	cp -f jc.h ~/include/
	cp -f libjc.a ~/lib/

testall: testall.cpp libjc.a jc.h Makefile
	g++ testall.cpp -o testall -I ~/include -L. -ljc $(CPPFLAGS)

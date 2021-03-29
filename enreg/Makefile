CPPFLAGS = -Wall -O3 -fPIC

libenreg.a: libenreg.o Makefile
	ar q libenreg.a libenreg.o

libenreg.o: libenreg.cpp enreg.h Makefile
	g++ -c libenreg.cpp -I . -I ~/include  $(CPPFLAGS)

install:
	cp -f libenreg.a ~/lib
	cp -f enreg.h ~/include

clean:
	rm -f libenreg.a libenreg.o call

call: call.cpp libenreg.a Makefile
	g++ call.cpp -o call -I ~/include -I . -L. -lenreg $(CPPFLAGS)


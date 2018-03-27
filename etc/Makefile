CPPFLAGS = -Wall -O3


libetc.a : libetc.o Makefile
	rm -f libetc.a
	ar q libetc.a libetc.o

libetc.o: libetc.cpp etc.h Makefile
	g++ -c libetc.cpp -I . -I ~/include $(CPPFLAGS)

call: call.cpp  libetc.a Makefile
	g++ call.cpp -o call -I . -L . -letc $(CPPFLAGS)

callD2: callD2.cpp  libetc.a Makefile
	g++ callD2.cpp -o callD2 -I ~/include -I . -L . -letc $(CPPFLAGS)

callHKd2: callHKd2.cpp  libetc.a Makefile
	g++ callHKd2.cpp -o callHKd2 -I ~/include -I . -L . -letc -L ../alea -lh -L ../filage -lfilage $(CPPFLAGS)

callHKd3: callHKd3.cpp  libetc.a Makefile
	g++ callHKd3.cpp -o callHKd3 -I ~/include -I . -L . -letc -L ../alea -lh -L ../filage -lfilage $(CPPFLAGS)

clean:
	rm -f libetc.a libetjc.o call *~

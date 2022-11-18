CC = g++
DFLAGS = -gdwarf-2 -g3 -O0 -D_GLIBCXX_DEBUG
CFLAGS = -std=c++17

dnsserver: dnsserver.cpp
	${CC} $^ ${DFLAGS} ${CFLAGS} -o $@

clean:
	rm -f dnsserver
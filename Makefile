CC = g++
DFLAGS = -gdwarf-2 -g3 -O0 -D_GLIBCXX_DEBUG
CFLAGS = -std=c++17

dnsserver: dnsserver.o DNSMessage.o
	${CC} $^ ${DFLAGS} ${CFLAGS} -o $@

dnsserver.o: dnsserver.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

DNSMessage.o: DNSMessage.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@


clean:
	rm -f dnsserver
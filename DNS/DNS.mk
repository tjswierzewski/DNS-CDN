CC = g++
DFLAGS = -gdwarf-2 -g3 -O0 -D_GLIBCXX_DEBUG
CFLAGS = -std=c++11

dnsserver: dnsserver.o DNSMessage.o DNSQuestion.o DNSResponse.o
	${CC} $^ ${DFLAGS} ${CFLAGS} -o $@

dnsserver.o: dnsserver.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

DNSMessage.o: DNSMessage.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

DNSQuestion.o: DNSQuestion.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

DNSResponse.o: DNSResponse.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

clean:
	rm -f dnsserver *.o
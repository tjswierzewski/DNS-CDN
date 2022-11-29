CC = g++
DFLAGS = -gdwarf-2 -g3 -O0 -D_GLIBCXX_DEBUG
CFLAGS = -std=c++17

httpserver: httpserver.o HTTPSession.o HTTPMessage.o HTTPMethod.o HTTPRequestMessage.o HTTPResponseMessage.o
	${CC} $^ ${DFLAGS} ${CFLAGS} -o $@

httpserver.o: httpserver.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

HTTPSession.o: HTTPSession.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

HTTPMessage.o: HTTPMessage.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

HTTPMethod.o: HTTPMethod.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

HTTPRequestMessage.o: HTTPRequestMessage.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

HTTPResponseMessage.o: HTTPResponseMessage.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

clean:
	rm -f httpserver *.o
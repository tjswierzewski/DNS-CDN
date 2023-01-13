CC = g++
DFLAGS = -gdwarf-2 -g3 -O0 -D_GLIBCXX_DEBUG
CFLAGS = -std=c++17

json.a: JSON.o JSONFloat.o JSONInt.o JSONJson.o JSONString.o JSONValue.o JSONList.o
	ar cr $@ $^

JSON.o: JSON.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

JSONFloat.o: JSONFloat.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

JSONInt.o: JSONInt.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

JSONJson.o: JSONJson.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

JSONString.o: JSONString.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

JSONValue.o: JSONValue.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

JSONList.o: JSONList.cpp
	${CC} -c $< ${DFLAGS} ${CFLAGS} -o $@

clean:
	rm -f *.a *.o
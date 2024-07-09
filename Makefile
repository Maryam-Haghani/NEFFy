CC=g++
CFLAGS=-O3
LDFLAGS=-static

prog=neff converter

all: ${prog}

neff: neff.cpp
	${CC} ${CFLAGS} -std=c++17 flagHandler.cpp common.cpp msaReader.cpp msaWriter.cpp msaSplitter.cpp $@.cpp -o $@
 
converter: converter.cpp
	${CC} ${CFLAGS} -std=c++17 flagHandler.cpp common.cpp msaReader.cpp msaWriter.cpp $@.cpp -o $@

install: ${prog}
	cp ${prog} ./bin
	
clean:
	rm ${prog}
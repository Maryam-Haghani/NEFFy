CC=g++
CFLAGS=-O3
LDFLAGS=-static

prog=neff converter

all: ${prog}

neff: neff.cpp
	${CC} ${CFLAGS} flagHandler.cpp common.cpp msaReader.cpp $@.cpp -o $@ ${LDFLAGS}

 
converter: converter.cpp
	${CC} ${CFLAGS} flagHandler.cpp common.cpp msaReader.cpp msaWriter.cpp $@.cpp -o $@ ${LDFLAGS}

install: ${prog}
	cp ${prog} ./bin
	
clean:
	rm ${prog}

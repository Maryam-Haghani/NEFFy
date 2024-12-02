CC=g++
CFLAGS=-O3
LDFLAGS=-static

prog=neff converter

all: ${prog}

neff: code/neff.cpp
	${CC} ${CFLAGS} -std=c++17 code/flagHandler.cpp code/common.cpp code/msaReader.cpp code/msaWriter.cpp code/multimerHandler.cpp code/neff.cpp -o neff

converter: code/converter.cpp
	${CC} ${CFLAGS} -std=c++17 code/flagHandler.cpp code/common.cpp code/msaReader.cpp code/msaWriter.cpp code/converter.cpp -o converter

install: ${prog}
	cp ${prog} ./bin
	
clean:
	rm -f ${prog}

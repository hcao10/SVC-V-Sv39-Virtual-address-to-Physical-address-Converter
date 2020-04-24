CC=g++ --std=c++11 -O3

PROG=do

all: cal 

main: cal.cpp
	$(CC) -o $(PROG) cal.cpp
exe:
	$(CC) -o $(PROG) cal.cpp
cgi:
	$(CC) -o cal.cgi cal.cpp

debug:
	g++ --std=c++11 -g -Wall -Wextra -o $(PROG) cal.cpp

clean:
	rm -rf *.o do

snazzy_shell: main.o function.o
	g++ -o snazzy_shell main.o function.o; rm *.o
function.o: function.cpp general.h
	g++ -Wall -c function.cpp
main.o: main.cpp general.h
	g++ -Wall -c main.cpp

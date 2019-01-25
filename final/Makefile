all:main

main: main.o parser.o
	g++ -o main main.o parser.o

.cpp.o: 
	g++ $< -c 


clean:
	rm -rf main *.o

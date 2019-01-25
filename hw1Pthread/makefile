total: main

main: hw1.cpp
	g++  -pthread -std=c++11 -O2 -s hw1.cpp -o pi
static: hw1.cpp
	g++  -static   -lm -lc -lpthread -std=c++11 -O2 -s hw1.cpp -o pi 
o2: hw1.cpp
	g++ -o pi hw1.cpp -lpthread -O2
noo2: hw1.cpp
	g++ -o pi hw1.cpp -lpthread 

clean:
	rm -rf main

total: main

OFLAGS = -fopenmp -O2

main: pi.cpp
	g++  $(OFLAGS) -std=c++11 -s pi.cpp -o pi

o2: pi.cpp
	g++ -o pi pi.cpp -lpthread -O2
noo2: pi.cpp
	g++ -o pi pi.cpp -lpthread

clean:
	rm -rf pi

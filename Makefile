all:
	g++ -o mean_shift.o mean_shift.cpp main.cpp -g --std=c++11 -I/usr/include/python2.7 -lpython2.7

clean:
	rm mean_shift.o
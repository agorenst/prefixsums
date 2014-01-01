all: prefix_sums

prefix_sums: prefix_sums.h prefix_sums.cpp
	g++ -Wall -fopenmp -O2 -std=c++0x prefix_sums.cpp -o prefix_sums
valgrind: prefix_sums
	valgrind --tool=drd ./prefix_sums
clean:
	rm -f *~ prefix_sums

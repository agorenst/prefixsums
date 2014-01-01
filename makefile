all: prefix_sums

prefix_sums: prefix_sums.h prefix_sums.cpp
	g++ -Wall -g -std=c++0x -fopenmp prefix_sums.cpp -o prefix_sums

valgrind: prefix_sums
	valgrind --tool=drd ./prefix_sums

#all: par

par: par.cpp par_fns.h par_utils.h array_utils.h tree_array.h prefix_sums.h
	g++ -g -Wall -std=c++0x -fopenmp par.cpp -o par
ser: par.cpp par_fns.h par_utils.h array_utils.h
	g++ -g -Wall -std=c++0x par.cpp -o ser
#valgrind:
#	valgrind --leak-check=yes ./par 100000


clean:
	rm -f *~ par ser prefix_sums

g++ -o qsearch pagerank.cpp pagerank.cc   -std=c++11 -L. -L/usr/lib/ -L/usr/local/lib   -lpthread 
cp qsearch  bin/
ls  -l  bin/qsearch

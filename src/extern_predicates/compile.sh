swipl-ld -std=c++11 -shared -ld g++ ./extern_predicates/index.cpp ./extern_predicates/h1.cpp -o ../bin/index.so
swipl-ld -std=c++11 -ld g++ tester.cpp -o ../bin/tester
g++ calc_speedup.cpp -o ../bin/calc_speedup

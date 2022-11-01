swipl-ld -std=c++11 -shared -ld g++ ./src/extern_predicates/index.cpp ./src/extern_predicates/h1.cpp -o index.so
swipl-ld -std=c++11 -ld g++ ./src/utils/tester.cpp -o tester
g++ ./src/utils/calc_speedup.cpp -o calc_speedup
g++ ./src/formula_generator/generator.cpp -o formula_generator
g++ ./src/formula_generator/espacios.cpp -o espacios

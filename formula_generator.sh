./espacios ./iscas85/$1.bench ./iscas85/$1.bench.clean
mkdir ./tests/$1
./formula_generator ./iscas85/$1.bench.clean ./tests/$1/$1

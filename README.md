# Demuba3

A Automatic Demonstrator written in Prolog

Copyright (C) 2022  David Emmanuel Lopez
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

# Instructions

* First, compile extern predicates with compile.sh or compile.bat. You need to have installed swi-prolog. For Ubuntu Linux, you can use "apt-get install swi-prolog" for install.
* Run demuba3 with demuba3.sh, the script load the foreign library library and consult demuba3.pl.
* Make a query, like "/- -(a & b) v (a & b):L. ". The query example ask if the formula " -(a & b) v (a & b) " is tautology, the L variable return the number of rules used by the demonstrator, when return sucess (tautology), that is for performance experiments.
* For batch test, you can use "./tester --dem=demuba3 --indexlib=index --set=c2670 --type=demostrart --limit=130 " as example. The tester must be compiled before. In this exmaple, the demuba3.pl demostrator will be used, the index.so/.dll library will be used, to test the tests case "tests/c2670" created by iscas85_formula_generator. The type of test is demostrart, in this kind of test, every formula in test cases will be used as "F v -F" to generate tautologies. Limit specify the number of test cases readed on test set.



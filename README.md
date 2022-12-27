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
* Make a query, like "/- -(a & b) v (a & b):L. "

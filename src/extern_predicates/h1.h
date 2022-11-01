
/*
    A Automatic Demonstrator written in Prolog
    Copyright (C) <2022  David Emmanuel Lopez
    http://www.github.com/davidemmanuellopez/demuba3

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
*/

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <set>
#include "ulong.h"

using namespace std;
static int l=-1;


class Nodo
{
	public:
		char operador;
		unsigned_long operando;
		int cambioPolaridad;
};

class Lista
{
	public:
		Nodo nodo;
		Lista *sig;
};

class Atomo
{
	public:
		
		int pol;
		string variable;
		int n;
		
		Atomo(const Atomo &at);
		Atomo(string v,int p);
		int intValue();
		string strValue();
		Atomo AtomoOpuesto();
		bool operator<(const Atomo& l) const;
		bool operator=(const Atomo&) const;
	
		void imprimir();
	};
	
	


void ramas(string str,unsigned_long* r, set<Atomo> &atomos, int pol);


	

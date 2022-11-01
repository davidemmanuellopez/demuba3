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
#include "h1.h"

using namespace std;

void Atomo::imprimir()
{
	//cout << "(" << variable << "," << pol << "," << n <<")";
	cout << n << ";";
	};
Atomo::Atomo(const Atomo &at)
{
	pol=at.pol;
	variable=at.variable;
	n=at.n;
	};
Atomo Atomo::AtomoOpuesto()
{
	int npol;
	if (pol==0) npol=1; else npol=0;
	Atomo na(variable,npol);
	return na;
	};
	
Atomo::Atomo(string v,int p)
{
	variable=v;
	pol=p;
	n = intValue();
};

int Atomo::intValue()
{
		string ns = variable.substr(1,variable.length());
		int nu=atoi(ns.c_str());
		if (pol==0) return -nu; else return nu;
	};			

string Atomo::strValue()
{
	string str = variable;
	if (pol==0) 
	{
		str+="=t";
		} else str+="=f";
	
	return str;
	};
bool Atomo::operator<(const Atomo& l) const
{
	return n < l.n;
};

bool Atomo::operator=(const Atomo& l) const
{
	return (pol==l.pol) && (variable == l.variable);
};

unsigned_long realizarOperacion(unsigned_long a,unsigned_long b, char op, int polaridad)
{

if (a<0 || b<0) cout << "WARNING, rama con valor negativo" << endl;
	if ((polaridad % 2)==1)
	{
		if (op=='+') op='*'; else
		{
			op='+';
			};
		};
	switch (op)
	{
		case '+': 
		return a+b;
		break;
		case '*': return a*b;
		break;
	
		};
		
return 0;
};


int cierra(string str, int i)
{
	
	if (i >= str.length()) return 1;
	while (str[i]==' ')i++;
    if (str[i]=='v' || str[i]==')' || str[i]=='(') return 1; else return 0; 
		
	
};

void initNodo(struct Nodo* nodo)
{
	nodo->operando=0;
	nodo->operador=' ';
	nodo->cambioPolaridad=0;
	};

void ramas(string str,unsigned_long* r, set<Atomo> &atomos, int pol)
{
int polaridad = pol;
//cout << "polaridad inicial:" << polaridad << endl;
Lista *lista = new Lista();
lista->sig = 0;
initNodo(&lista->nodo);
lista->nodo.cambioPolaridad=polaridad;
*r=0;
int dpol=0;
int p=0;

//	cout << "largo formula:" << str.length() << endl;
//	cout << "limite l:" << l << endl;
	while (p < str.length()) //interpretar
	{

		char caracter = str[p];
//		printf("caracter %c \n",caracter);
		Lista* dirvieja;
		
		if (caracter=='v')
		{
//			cout << "operador +" << endl;
			lista->nodo.operador='+';
			} else
		{
			if ((caracter=='p')) //si es operando
			{
				int i=p;
				p++;	
//				cout << "operando " << str[p] << endl;
				while ((str[p] >='0') && (str[p] <='9')) p++;
				p--;
				int j=p;
				
				//guardar el atomo en la lista
				string v = str.substr(i,j-i+1);
//				cout << "variable:" << v << endl;
				
				int pola;
				
				//Atomo* at = new Atomo(v,(polaridad+dpol) % 2);
				Atomo at(v,(polaridad+dpol) % 2);
				dpol=0;
			//	cout << "polaridad:" << polaridad << endl;
//				cout << "que poronga estoy insertando?" << endl;
			//	at->imprimir();
				atomos.insert(at);
				
				
				if (lista->nodo.operador!=' ' && !cierra(str,p+1)) //tiene operador y no cierra...
				{
//				cout << "tiene operador y no cierra" << endl;
//				printf("apilo \n");
				//sigue un * o &
				// tengo que apilar la operacion que queria hacer hasta tener el operando completo...
				
				//si reemplazo este procedimiento de apilar, por realizar la operacion y acumularla en la misma lista?
				/*	
				dirvieja = lista;
				
				lista = new Lista();
				lista->sig = dirvieja;
				
				//limpiar (esto despues lo puedo hacer usando el mismo nodo
				initNodo(&lista->nodo);
				
				//es una variable, entonces seteo su valor
				lista->nodo.operando=1;
				
				 */
				lista->nodo.operando=realizarOperacion(lista->nodo.operando,1,lista->nodo.operador,polaridad);
				lista->nodo.operador=' ';
				
				} else
				
				{
					if (lista->nodo.operador!=' ' && cierra(str,p+1)) //tiene operador y cierra
					{
//						cout << "tiene operador y cierra" << endl;
//						cout << "posicion i:" << p << endl;
//						printf("cierra y resuelvo operacion");
						lista->nodo.operando=realizarOperacion(lista->nodo.operando,1,lista->nodo.operador,polaridad);
//						cout << "realizo operacion" << endl;
						lista->nodo.operador=' '; //nuevo
						if (l!= -1 && lista->nodo.operando > l)
						{
//							cout << "sale prematuramente" << endl;
							*r=lista->nodo.operando;
							return;
						};
						//aca desapilo?
						
						
					} else
					{
//						cout << "operando es 1" << endl;
//						printf("operando %i es uno \n",caracter);
						lista->nodo.operando=1;
						//q pasa is aca resuelvo con el nodo anterior?
/*						lista = lista->sig;
						cout << "aca realizo la op" << endl;
						lista->nodo.operando=realizarOperacion(lista->nodo.operando,1,lista->nodo.operador,polaridad);
	*/					
					}
				};
						
				};
			};
		
	
			 
		switch (caracter)
		{
		case '&':
			lista->nodo.operador='*';
			break;
		
		case '(':
				//tengo que apilar el estado actual
//				cout << "abre parentesis, creo nueva lista" << endl;
				
				dirvieja = lista;
				
				lista = new Lista();
				lista->sig = dirvieja;
				
				//limpiar (esto despues lo puedo hacer usando el mismo nodo
				initNodo(&lista->nodo);
				lista->nodo.cambioPolaridad=polaridad+dpol;
				polaridad=polaridad+dpol;
//				cout << "polaridad apilada:" << lista->nodo.cambioPolaridad << endl;
				dpol=0;
				
			break;
		case ')':
		
				{
//				cout << "Cerrar parentesis" << endl;
				int operandoAnt;
				operandoAnt = lista->nodo.operando;
		
				Lista *sig = lista->sig;
				delete lista;
				lista=sig;	
//				cout << "desapilo lista" << endl;
//				cout << "polaridad recuperada:" << lista->nodo.cambioPolaridad << endl;
				if (lista->sig==0) 
				{
//					cout << "cuidado" << endl;
					};
					
		
				/*
				if (lista->nodo.cambioPolaridad==1)
				{
					polaridad--;
				};
				*/
			
				polaridad=lista->nodo.cambioPolaridad;
				//si el termino entre parentesis no es operando de una operacion, es un cambio de polaridad
				// tengo que verificar eso
			
				if (lista->nodo.operador!=' ')
				{
//					cout << "realiza operacion al salir del )" << endl;
					lista->nodo.operando=realizarOperacion(lista->nodo.operando,operandoAnt,lista->nodo.operador,polaridad);
					if (l!= -1 && lista->nodo.operando > l)
					{
					//	cout << "sale prematuramente" << endl;
					    *r=lista->nodo.operando;
						return;
						};
					} 
				else
				{				
//					cout << "no realiza operacion" << endl;	
						lista->nodo.operando = operandoAnt;
				}
				};	
				break;	
							
		case '-':
//				cout << "posicion " << p << ",incremento polaridad" << endl;
				dpol=1;
				
				int s=p+1;
				while (str[s]==' ' || str[s]=='-') 
				{
					if (str[s]=='-') 
					{
						dpol++;
//						cout << "vuelvo a incrementar" << endl;
						};
					s++;
					};
				

				p=s-1;
				break;

		
		};
		
		p++;
	};
	
	while (lista->sig!=0)
	{
		long operandoAnt=lista->nodo.operando;
		delete lista;
		lista = lista->sig;
		/*
		if (lista->nodo.cambioPolaridad==1)
				{
					polaridad--;
				};
				 */
		polaridad= lista->nodo.cambioPolaridad;
		lista->nodo.operando=realizarOperacion(lista->nodo.operando,operandoAnt,lista->nodo.operador,polaridad);
		};
	*r=lista->nodo.operando;
	delete lista;
};
// predicado elegir, que dada una lista de formulas las devuelve en orden
// 

	

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
#include <fstream>
#include <list>
#include <map>
#include <set>
#include <stdlib.h>

using namespace std;
class Gate;
static set<Gate*> pisados;

map<string,Gate*> mapa;
static int prof =0;
string reverseGet(map<string,Gate*> mapa, Gate* g)
{
	map<string,Gate*>::iterator it = mapa.begin();
	
	while (it->second!=g && it!=mapa.end()) it++;
	return it->first;
	};
	
	
list<string> split(string input, string delimiter)
{
	list<string> l;
	size_t pos = 0;
	string token;
	while ((pos = input.find(delimiter))!=string::npos)
	{
		token = input.substr(0,pos);
		l.push_back(token);
		input.erase(0,pos+ delimiter.length());
		};
	l.push_back(input);
	return l;
	};
	
class Gate
{
	public:
		virtual void convertir(ofstream &fsalida)
		{};
		
	};


class GateRec: public Gate
{
	public:
	virtual void addGate(Gate g)
	{
		};
};
class Var: public Gate
{
	private:
	string var;
	
	public:
	
		Var(string v)
		{
			var=v;
			};
		void convertir(ofstream &fsalida)
		{
			fsalida << "p"<< var;
			};
	};

	
class Or: public GateRec
{

	private:
		list<Gate*> ors;
			
	public:
	
	void addGate(Gate* g)
	{
		ors.push_back(g);
		};
		
	void convertir(ofstream &fsalida)
	{
//		cout << reverseGet(mapa,this) << endl;
		if (pisados.find(this)==pisados.end())
		{
			cout << "pisados: " << pisados.size() << "/" << mapa.size() << endl;
			pisados.insert(this);
			} else
			{
	//			cout << "ciclo" << endl;
				};
				
		string salida;
		list<Gate*>::iterator it = ors.begin();
		Gate* g = *it;
	    g->convertir(fsalida);
		it++;
		while (it!=ors.end())
		{
			g = *it;
			fsalida << " v "; 
			prof++;
			g->convertir(fsalida);
			prof--;
			it++;
			};
		
			//pisados.erase(this);
	};
			
};

class NOr: public Or
{
	void convertir(ofstream &fsalida)
		{
			fsalida << "-(";
			Or::convertir(fsalida);
			fsalida << ")";
		}
	};

class And: public GateRec
{
	public:	

	list<Gate*> ands;
	void addGate(Gate* g)
	{
		ands.push_back(g);
		};
	
	void convertir(ofstream &fsalida)
	{
//		cout << reverseGet(mapa,this) << endl;
				if (pisados.find(this)==pisados.end())
		{
			cout << "pisados: " << pisados.size() << "/" << mapa.size() << endl;
			pisados.insert(this);
			} else
			{
		//		cout << "ciclo" << endl;
				};
		
		string salida;
		list<Gate*>::iterator it = ands.begin();
		Gate* g = *it;
		fsalida << "(";
		g->convertir(fsalida);
		fsalida << ")";
		it++;
		while (it!=ands.end())
		{
			g = *it;
			fsalida << " & (";
			prof++;
			g->convertir(fsalida);
			fsalida << ")";
			it++;
			};
	
	//		pisados.erase(this);
	};
	
};
	
class NAnd: public And
{
	void convertir(ofstream &fsalida)
	{
			fsalida << "-(";
			And::convertir(fsalida);
			fsalida << ")";
	}
};

class XOr: public GateRec
{
	public:
	
	list<Gate*> terms;
	void addGate(Gate* g)
	{
		terms.push_back(g);
		};
		
	void convertir(ofstream &fsalida)
	{
		
		Gate* g1 = *terms.begin();
		Gate* g2  = *(terms.begin()++);
		
		fsalida << "(";
		g1->convertir(fsalida);
		
		fsalida << ") & -(";
		g2->convertir(fsalida);
		fsalida << ") v -(";
		g1->convertir(fsalida);
		fsalida << ") & (";
		g2->convertir(fsalida);
		fsalida << ")"; 

	};

};
class Not: public Gate
{
	private:
	Gate* g;
	
	public:
	Not(Gate* g)
		{
			this->g=g;
			};
	void convertir(ofstream &fsalida)
	{
//		cout << reverseGet(mapa,this) << endl;
		if (pisados.find(this)==pisados.end())
		{
			cout << "pisados: " << pisados.size() << "/" << mapa.size() << endl;
			pisados.insert(this);
			} else
			{
//				cout << "ciclo" << endl;
				};
		fsalida << "-(";
		prof++;
		g->convertir(fsalida);
		prof--;
		fsalida << ")";
//		pisados.erase(this);
	};

};


int main(int argc, char* argv[])
{

	
	
	string entrada(argv[1]);
	string salida(argv[2]);

	ifstream fentrada(entrada.c_str());
	
	string linea;
	list<string> outputs;
	int coutputs=0;
	while (getline(fentrada,linea))
	{
		
		if (linea[0]=='I')
		{
			int i = linea.find_first_of('(');
			string id = linea.substr(i+1,linea.length()-i-2);
//			cout << "id: " << id << endl;
			Gate* g = new Var(id);
			mapa.insert(pair<string,Gate*>(id,g));
			} 
			
		if (linea[0]=='O') // es output
		{
			int p = linea.find_first_of('(')+1;
			int j = linea.find_first_of(')');
			string idGate = linea.substr(p,j-p);
			cout << "OUTPUT:" << idGate << endl;
			outputs.push_back(idGate);
			coutputs++;
			} else
		//es gate
		{
//			cout << "es gate" << endl;
			int p = linea.find_first_of('=');
			string idGate = linea.substr(0,p);
//			cout << "idgate:" << idGate << endl;
			p=p+1;
			int j = linea.find_first_of('(');
			string gate = linea.substr(p,j-(p));
//			cout << "gate:" << gate << endl;
			p=j+1;
			j=linea.length()-1;
			
			string recorte = linea.substr(p,j-p);
//			cout << "recorte:" << linea.substr(p,j-p) << endl;
			list<string> tokens = split(recorte,",");
			
			list<string>::iterator it = tokens.begin();
//			cout << "tokens:" << endl;
			Gate* g;
			if (gate=="BUFF")
			{
				cout << "es buff" << endl;
				string a = *it;
				cout << a << endl;
				map<string,Gate*>::iterator s = mapa.find(a);
				g = s->second;
				};
			if (gate=="AND")
			{
//				cout << "procesar gate AND:" << idGate << endl;
				And* a = new And();
				g = a;
				while (it!=tokens.end())
				{
					map<string,Gate*>::iterator s = mapa.find(*it);
					
					
					
					Gate* gh = s->second;
					a->addGate(gh);
					it++;
					};
			};
				
			if (gate=="NAND")
			{
				NAnd* a = new NAnd();
				g = a;
				while (it!=tokens.end())
				{
					map<string,Gate*>::iterator s = mapa.find(*it);
		
					Gate* gh = s->second;
					a->addGate(gh);
					it++;
					};
				
			};
			
			if (gate=="NOR")
			{
				NOr* a = new NOr();
				g = a;
				while (it!=tokens.end())
				{
					map<string,Gate*>::iterator s = mapa.find(*it);
//					cout << "*it:" << *it << endl;
//					cout << "elementO:" << s->first << endl;
					Gate* gh = s->second;
					a->addGate(gh);
					it++;
				};
			};	
			
			if (gate=="OR")
			{
				Or* a = new Or();
				g = a;
				while (it!=tokens.end())
				{
					map<string,Gate*>::iterator s = mapa.find(*it);
//					cout << "*it:" << *it << endl;
//					cout << "elementO:" << s->first << endl;
					Gate* gh = s->second;
					a->addGate(gh);
					it++;
				};
			};
			
			if (gate=="XOR")
			{
				XOr* a = new XOr();
				g = a;
				while (it!=tokens.end())
				{
					map<string,Gate*>::iterator s = mapa.find(*it);
//					cout << "*it:" << *it << endl;
//					cout << "elementO:" << s->first << endl;
					Gate* gh = s->second;
					a->addGate(gh);
					it++;
				};
			};
			
			if (gate=="NOT")
			{

//				cout << "NOt:" << endl;
				map<string,Gate*>::iterator s = mapa.find(*it);
				Gate* gh = s->second;
//				cout << "gh:" << gh->convertir() << endl;
				g = new Not(gh);
			};
			
							
//			cout << "inserto en mapa:" << endl;
//			cout << g->convertir() << endl;
			cout << "idGate:" << idGate <<",direccion:" << g << endl;
			mapa.insert(pair<string,Gate*>(idGate,g));
			}
		
		};
		
	
	list<string>::iterator itouts = outputs.begin();
	while (itouts!=outputs.end())
	{
		string out = *itouts;
		
		//crear archivo y escribir formula de salida (invocacion a convertir)
		ofstream fsalida(salida.c_str()+out);
			
			map<string,Gate*>::iterator s = mapa.find(out);
			Gate* g = s->second;
			cout << "out:" << out << endl;
			g->convertir(fsalida);
			
		fsalida << "." << endl;
		fsalida.close();
		itouts++;
		
		}; 

	
	
}

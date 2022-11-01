
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

#include<SWI-cpp.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include "h1.h"
#include <stack>
#include <vector>

#define CHECK 

/*
#define DEBUG_ELIMINAR
#define DEBUG_DESELIMINAR
#define DEBUG_DESACTUALIZAR
#define DEBUG_INDEXAR
#define DEBUG_ACTUALIZAR
* */
namespace std
{
    template<> struct less<term_t>
    {
       bool operator() (const term_t& lhs, const term_t& rhs) const
       {
           return &lhs < &rhs;
       }
    };
}

using namespace std;

void imprimir_atomos(set<Atomo> s);

void diferencia(set<Atomo> &at1, set<Atomo> &at2, set<Atomo> &diff)
{
	set<Atomo>::iterator it = at2.begin();
	diff.insert(at1.begin(),at1.end());
	while (it!=at2.end())
	{
		Atomo at = *it;
		
		set<Atomo>::iterator itb = diff.find(at);
		if (itb!=diff.end())
		{
			diff.erase(itb);
			};
		it++;
		};
	};
	/*
set<Atomo>* Union(set<Atomo> &at1, set<Atomo> &at2)
{
	set<Atomo>::iterator it = at2.begin();
	set<Atomo> r = new set<Atomo>(at1);
	r->insert(at2.begin(),at2.end());
	return r;
	};
	*/

Atomo str2atom(string satomo)
{
	int p = satomo.find_first_of("=");
	int pol=0;
	if (satomo[p+1]=='f') pol=1;
	string var = satomo.substr(0,p);
	
	Atomo at(var,pol);
	return at;
	};


class EHeap
{
	public:
	unsigned_long r;
	int  i;
	EHeap(long int r, int i);
	bool operator<(const EHeap& l) const;
	};
	
	
struct Actualizacion
{
	set<Atomo>* setDiff;
	int i;
	unsigned_long r;
	term_t formula;
	};

struct Eliminacion
{
	int i;
	unsigned_long r;
	set<Atomo> atomos;
	set<Atomo> atomosEliminados;
	term_t formula;
	};
	
struct Estado
{
		int indiceMax;
		vector<Actualizacion> actualizaciones;
		vector<Eliminacion> eliminaciones;
		vector<Eliminacion> deseliminar;

		set<Atomo> cache;
	};

bool EHeap::operator<(const EHeap& l) const
{
	return r < l.r;
};

EHeap::EHeap(long int r, int i)
{
	this->r = r;
	this->i = i;
	};

class Index
{
	public:
	int n; 
	Index();
	Index(const Index& i);
	~Index();
	void indexF(PlTerm f);
	void parseF(string f, set<Atomo> &atomos, unsigned_long* r);
	void newCache();
	void putAtomo(Atomo at);
	void formulasAfectadas(set<int> &f);
	void actualizarF(int indice,PlTerm f);
	void eliminarF(Eliminacion &e);
	long int elegir();
	long int elegirR();
	void mkElegir(Atomo at, bool s);
	void mkEliminacion(int i,Eliminacion &e);
	
	private:
	
		map<Atomo, set<int> > dmap;
		map<int, set<Atomo> > rmap;
		map<int, set<Atomo> > atEliminados;
		map<Atomo, set<int> > atEinv;
		map<int, unsigned_long> hmap;
		set<Atomo> cacheAtomos; 
		map<int,term_t> formulas;
		vector<EHeap> velegir;
		vector<EHeap>::iterator itelegir;
		vector<EHeap>::reverse_iterator itrelegir;
		int indice_cont;
		
	};

Index::Index()
{
	n=0;
	indice_cont=0;
};

Index::~Index()
{
/*
		for ( const auto &myPair : dmap ) {
			set<int> s = myPair.second;
			s.clear();

		}
		
		for ( const auto &myPair : rmap ) 
		
		{
			set<Atomo> s = myPair.second;
			s.clear();

		}


	dmap.clear();
	rmap.clear();
	hmap.clear();
	velegir.clear();
	cacheAtomos.clear();*/
	};

Index::Index(const Index& i)
{
	n = i.n;	
	dmap = i.dmap;
	rmap = i.rmap;
	hmap = i.hmap;
	cacheAtomos = i.cacheAtomos;
	atEliminados = i.atEliminados;
	atEinv =i.atEinv;
	formulas = i.formulas;
    indice_cont=0;
	velegir.clear();
	};

void Index::mkElegir(Atomo at, bool s)
{

	//lleno el vector, con las formulas que tienen al atomo (dmap)
	map<Atomo, set<int> >::iterator itm; // <-- iterator para DirectMap (Atomo, set<Indices>)
	itm = dmap.find(at);
	if (itm!=dmap.end()) //Existe entrada para el atomo necesario
	{

		set<int> indices = (itm->second); //Obtengo los indices que contienen al atomo
	
		set<int>::iterator itIndices = indices.begin();
		
		while (itIndices!=indices.end())
		{
			
			unsigned_long r = (hmap.find(*itIndices)->second); // Cantidad de ramas para el indice actual
			
			EHeap e(r,*itIndices); //Construyo elemento heap para ordenar
		
			velegir.push_back(e);
			//cout << "inserto "<< endl; 
			itIndices++;
		};
	}
//	Busco en AtomosEliminados (Invertido)
/*
//	at.imprimir();
	itm = atEinv.find(at);
	if (itm!=atEinv.end()) //Si existe entrada para el Atomo
	{
	
		set<int> indices = (itm->second); //Indices que contienen al atomo en estado eliminado
	
		set<int>::iterator itIndices = indices.begin();
		
		while (itIndices!=indices.end())
		{
			unsigned_long r = (hmap.find(*itIndices)->second);
			
			EHeap e(r,*itIndices);
			velegir.push_back(e);
			itIndices++;
		};
	}
	*/
	if (velegir.size()==0)
	{
		//cout << "no hay formula con atomo opuesto, insertar todas" << endl;
		//insertar solo la que tiene mayor indice

		
		 for ( const auto &myPair : hmap ) {
			 EHeap e(myPair.second,myPair.first);
			velegir.push_back(e);

		}
	}
    /*
		 for (map<int, ulong>::iterator ithmap=hmap.begin(); ithmap!=hmap.end(); ++ithmap)
		 {
			EHeap e(ithmap->second,ithmap->first);
			velegir.push_back(e);
		};
		*/
		
	if (s) sort(velegir.begin(),velegir.end()); else
	{
		itrelegir = velegir.rbegin();
		};
	itelegir= velegir.begin();

	};
	
void Index::newCache()
{
	cacheAtomos.clear();
	};
	
long int Index::elegir()
{
	
	if (indice_cont!=0) return indice_cont;
	//vector<EHeap>::iterator itelegir = itelegir;
	if (itelegir== velegir.end()) return -1;
	int v = itelegir->i;
//	cout << "indice:" << v << endl;
//	cout << "ramas:" << itelegir->r << endl;
	itelegir++;
	return v;
	};
	
long int Index::elegirR()
{
	
	if (indice_cont!=0) return indice_cont;
	//vector<EHeap>::iterator itelegir = itelegir;
	if (itrelegir== velegir.rend()) return -1;
	int v = itrelegir->i;
//	cout << "indice:" << v << endl;
//	cout << "ramas:" << itelegir->r << endl;
	itrelegir++;
	return v;
	};
	
void Index::eliminarF(Eliminacion &e)
{
	
	//Elimino la formula si no aporta informacion para generar contradiccion (es tautologia)
	
	/* pre:
	 * la formula esta indexada
	 * existe entrada en rmap, y contiene al menos un atomo en set<Atomo> (sino, estaria eliminada)
	 * existe las entradas por atomo en dmap, y puede darse que haya que eliminarlas si
	 * la formula en cuestion era la unica que indexaba a ese atomo
	 * 
	 * Si F tiene atomos eliminado, existe entrada en AtEliminados, y 
	 * existe entrada por cada atomoE en atEinv
	 * 
	 * */
	 
#ifdef DEBUG_ELIMINAR
	cout << "ELIMINAR :" << e.i << endl;
#endif
	//proceso eliminacion
	set<Atomo>::iterator itAts = e.atomos.begin();
//	cout << "proceso de eliminacion:" << endl;
	while (itAts!=e.atomos.end()) //por cada atomo indexado
	{
		Atomo at = *itAts;
//		at.imprimir();
		//obtengo conj de formulas que contienen al atomo
		set<int>* fs = &(dmap.find(at)->second);
		fs->erase(e.i); //elimino la formula actual
		if (fs->size()==0) //si no hay mas formulas que contengan al atomo, elimindo la entrada en dmap
		{
			dmap.erase(at);
			};
		
		itAts++;
	};
		
	//elimino la entrada de rmap(que ya la guarde en la estructura)
	rmap.erase(e.i);
	//elimino de hmap
	hmap.erase(e.i);
		
	if (e.atomosEliminados.size() > 0)
	{
//	cout << " toma el puntero de atomosEliminados" << endl;
	set<Atomo>::iterator itatE = e.atomosEliminados.begin();
	while(itatE!=e.atomosEliminados.end())
	{
		Atomo at = *itatE;
//		at.imprimir();
		//tengo que eliminar de atEinv la indexacion de la formula, y si era la ultima F, eliminar la entrada
		set<int>* fs = &(atEinv.find(at)->second);
		if (fs->size()==1)
		{
			atEinv.erase(at);
			} else fs->erase(e.i);
		itatE++;
	};	
	atEliminados.erase(e.i);
	};
	

};
void Index::actualizarF(int indice,PlTerm f)
{
#ifdef DEBUG_ACTUALIZAR	
	cout << "ACTUALIZAR :" << indice << endl;
#endif

 bool check;
#ifdef CHECK
   
    //Cond. 1 la formula tiene que estar indexada
    check = (rmap.find(indice)!=rmap.end());
    if (!check)
    {
		     cout << "FORMULA A ACTUALIZAR NO ESTA INDEXADA" << endl;
		     exit(1);
		};
#endif
	/*
	 * tengo que hacer diff, y agregar a atomosEliminados diff
	 * a su vez, todos los atomos de diff indexarlos en atEinv
	 * 
	 */
	string formula = (char*) f;
	set<Atomo> at;
	unsigned_long r;//vuelvo a parsear la formula

	parseF(formula,at,&r);
	if (r==0) //formula contradictoria
	{
		//cout << "indice cont actualizarF" << endl;
		indice_cont=indice;
		return;
		};
#ifdef CHECK
	
	//Cond. 2 la formula tiene que tener atomos
	/*
	check = (at->size()>0);
	 if (!check)
    {
		     cout << "FORMULA NO TIENE ATOMOS" << endl;
		     exit(1);
		};
	*/
	
	//Cond. 3 todos los atomos actuales, ya fueron indexados en el estado anterior de la formula
	set<Atomo> indexacion = (rmap.find(indice)->second);
	set<Atomo>::iterator itAtomos = at.begin();
	while (itAtomos != at.end())
	{
		Atomo at = *itAtomos;
		check = (indexacion.find(at)!=indexacion.end());
		if (!check)
		{
			cout << "ATOMOS EN ACTUALIZACION NO INDEXADOS PREVIAMENTE, PROB. FORMULA MAL ASOCIADA" << endl;
			exit(0);
			};
		itAtomos++;
		};
	
#endif	

	map<int,set<Atomo> >::iterator itrmap,itatE;
	itrmap= rmap.find(indice);
	//tomo el set de atomos que ya tenia guardado de la formula
	set<Atomo>* atOld = &(itrmap->second);
//	imprimirAt(*atOld);
	set<Atomo> diff;
	//calculo los atomos que fueron eliminados
	diferencia(*atOld,at,diff);
	//set_difference(atOld->begin(),atOld->end(),at->begin(),at->end(),diff->begin());
	//busco atomos que ya tenia eliminados la formula para hacer union con los
	//nuevos atomos eliminados
	//busco el set de atomos eliminados con anterioridad
	/*
	itatE= atEliminados.find(indice);
	set<Atomo> u;
	if (itatE!=atEliminados.end()) // si existe
	{
			set<Atomo>* eOld = &(itatE->second);
			u = *Union(*eOld,diff);
			atEliminados.erase(itatE);
			
		} else u=diff; //sino es la diferencia actual
	atEliminados.insert(pair<int, set<Atomo> >(indice, u));
	//actualizo atomos actuales de la formula
	* */
	rmap.erase(indice);
	rmap.insert(pair<int, set<Atomo> >(indice,at));
	//tengo que quitar la formula indexada en todos los atomos que estan en diff
	//los atomos que fueron eliminados (que igual se guardan en atEliminados)
	//y ponernos en atEliminadosInv
	
	
	set<Atomo>::iterator itDiff = diff.begin();
	while (itDiff!=diff.end())
	{
//		cout << "iterando en diff" << endl;
		Atomo at = *itDiff;
		map<Atomo,set<int> >::iterator itdmap,itatEi;
		itdmap = dmap.find(at);
		set<int>* l = &(itdmap->second);
		l->erase(indice); //quito la formula en la indexacion de los atomos que quite
		if (l->size()==0) dmap.erase(at); //si ya no hay formula que indexe el atomo, elimino la entrada
//		cout << "inserto en atEliminadosInv para tener la indexacion que me permita elegir" << endl;
	itDiff++;
	}
		/*
		set<int> s;
		itatEi = atEinv.find(at);
		if (itatEi==atEinv.end())
		{
//			cout << "tengo que crear la entrada" << endl;
			s.insert(indice);
			atEinv.insert(pair<Atomo,set<int> >(at,s));
			} else itatEi->second.insert(indice);
		//s->insert(indice);
		itDiff++;
		};*/
//	cout << "tengo que actualizar la cantidad de ramas en hmap" << endl;
	hmap.erase(indice);
	hmap.insert(pair<int,unsigned_long>(indice,r));

	};
void Index::formulasAfectadas(set<int> &u)
{
	//por cada atomo (con polaridad positiva o negativa) en cacheAtomos, hago la union con cada set de formulas en dmap
	set<Atomo>::iterator it = cacheAtomos.begin();
//	cout << "interacion cacheAtomos:" << endl;
	while (it!=cacheAtomos.end())
	{
		Atomo at = *it;
	//	at.imprimir();
		Atomo ato = at.AtomoOpuesto();
		map<Atomo,set<int> >::iterator itm;
		//busco formulas afectadas por el atomo
		itm = dmap.find(at);
		if (itm!=dmap.end())
		{
			set<int> s = (itm->second);
			u.insert(s.begin(),s.end());		
			};
		//busco formulas afectadas por el atomo con polaridad opuesta
		itm = dmap.find(ato);
		if (itm!=dmap.end())
		{
			set<int>* s = &(itm->second);			
			u.insert(s->begin(),s->end());		
			};
		it++;
	}; 
};
void Index::parseF(string f, set<Atomo> &atomos, unsigned_long* r)
{
	string formula= f;
	int pol=0;
	int p = formula.find_first_of("=");
	if (formula[p+1]=='f') pol=1;
	string subf = formula.substr(0,p);
	if (subf.size()==1)
	{
		if (subf[0]=='t')
		{
			if (pol==1)
			{
				*r=0;
				return;
				};
			}
		else
		{
			if (pol==0)
			{
				*r=0;
				return;
			};
		};
	} else ramas(subf,r,atomos,pol);
	
};

void Index::indexF(PlTerm f)
{

	string formula = (char*) f;
#ifdef DEBUG_INDEXAR
	//cout << "INDEXAR :" << formula << endl;
#endif	
	set<Atomo> atomos;
	unsigned_long r;
	parseF(formula,atomos,&r); //parse f le das una formula y devulve los atomos y la cantidad de ramas
	hmap.insert(pair<int,unsigned_long>(n,r));
	//inserto atomos
	rmap.insert(pair<int,set<Atomo> >(n,atomos));
	//term_t tc = PL_copy_term_ref((term_t) f);
	//ahora indexo a la formula desde cada atomo
	set<Atomo>::iterator it = atomos.begin();
	while (it!=atomos.end()) //por cada atomo, indexo la formula
	{
		Atomo at =*it;
		map<Atomo,set<int> >::iterator itm;
		itm = dmap.find(at);
		set<int> s;
		if (itm==dmap.end()) //si no existe la entrada, la creo
		{
		//	cout << "no existe entrada, la creo" << endl;
			s.insert(n);
			dmap.insert(pair<Atomo,set<int> >(at,s));
			} else itm->second.insert(n);
		it++;
		};
	};
		

void Index::putAtomo(Atomo at)
{
	cacheAtomos.insert(at);
	//imprimir_atomos(cacheAtomos);
	};
void Index::mkEliminacion(int i,Eliminacion &e)
{
	//guarda r, atomos, y atomosEliminados de una formula i
	e.i = i;
	e.r = (hmap.find(i)->second);
	e.formula = formulas.find(i)->second; //obtiene term_t de la formula dada por el indice
	map<int, set<Atomo> >::iterator itrmap,itatE;
	itrmap = rmap.find(i);
	set<Atomo> ats = (itrmap->second); //atomos en rmap
	e.atomos = ats;
	itatE = atEliminados.find(i); //guardo los atomos eliminados de la formula en la estructura Eliminacion
	if (itatE!=atEliminados.end())
	{
		e.atomosEliminados = (itatE->second);
		} 
	
	};	
static set<Index*>* indices;

PREDICATE(mkIndex,1)
{
	indices = new set<Index*>();
	Index* i = new Index();
	indices->insert(i);
	//cout << "n: " << i->n << endl;
	return A1 = (void*) i;
	};
	
PREDICATE(rmIndex,1)
{
	Index *i = (Index*) (void*) A1;
	indices->erase(i);
	delete(i);
	return TRUE;
	};
	
PREDICATE(sizeIndices,1)
{
	return A1 = (int) indices->size();
	};
	
PREDICATE(cpIndex,2)
{
	Index *oi = (Index*) (void*) A1;
	Index *ni = new Index(*oi);
	indices->insert(ni);
	return A2 = (void*) ni;
	};
	

//indexarF(index,Fold,Fnew)
/* dada una formula reducida, me da indexa con sus nuevos atomos, y los atomos eliminados
 * haciendo operacion de conjunto
 * y almacenando la cantidad de ramas*/
PREDICATE(indexarF,2)
{
	Index *i = (Index*) (void*) A1;
	//tomo el index
	PlTerm* formula = new PlTerm(A2);
	i->n++;
	i->indexF(*formula);
	delete formula;
	return TRUE;
	};
	
PREDICATE(newCache,1)
{
	Index *i = (Index*) (void*) A1;
	i->newCache();
	return TRUE;
	};
	

PREDICATE(putAtomo,2)
{
	Index *i = (Index*) (void*) A1;
	string satomo = (char*) A2;
	int p = satomo.find_first_of("=");
	int pol=0;
	if (satomo[p+1]=='f') pol=1;
	string var = satomo.substr(0,p);
	Atomo at(var,pol);
	i->putAtomo(at);
	return TRUE;
	};	

//toma la lista de atomos acumulados y devuelve una lista de indices a formulas 
//afectadas por esos atomos
PREDICATE(formulasAfectadas,2)
{
		Index *i = (Index*) (void*) A1;
	
	set<int> formulas;
	i->formulasAfectadas(formulas);
	
	//construir lista de prolog
	PlTail l(A2);
	
	set<int>::iterator it = formulas.begin();
	
	while (it!=formulas.end())
	{
		long int i = *it;
		PlTerm t(i);
		l.append(t);
		it++;
		};
	l.close();
	return TRUE;
	};

//dado un indice y su formula correspondiente reducida, actualiza la indexacion
//y guarda el estado anterior para recuperar a la vuelta de la recursion
PREDICATE(actualizarF,3)
{
	
	Index *i = (Index*) (void*) A1;
		
	int indice = (int) A2;
	i->actualizarF(indice,(PlTerm) A3);
	return TRUE;
};

PREDICATE(mkElegir,2)
{
	Index *i = (Index*) (void*) A1;
	Atomo at = str2atom((char*) A2);
	i->mkElegir(at,true);
	return TRUE;
	
	};
	
PREDICATE(mkElegirNh,2)
{
	Index *i = (Index*) (void*) A1;
	Atomo at = str2atom((char*) A2);
	i->mkElegir(at,false);
	return TRUE;
	
	};
PREDICATE(elegir,2)
{
	Index *i = (Index*) (void*) A1;
	long int indice = i->elegir();
	if (indice!=-1) return A2=(PlTerm) indice; else return FALSE;
	};

PREDICATE(elegirR,2)
{
	Index *i = (Index*) (void*) A1;
	long int indice = i->elegirR();
	if (indice!=-1) return A2=(PlTerm) indice; else return FALSE;
	};

PREDICATE(eliminarF,2)
{
	Index *i = (Index*) (void*) A1;
	int indice = (int) A2;
	Eliminacion e;
	i->mkEliminacion(indice,e);
	i->eliminarF(e);
	return TRUE;
	};

void imprimir_atomos(set<Atomo> s)
{
	
	set<Atomo>::iterator it = s.begin();
	while (it!=s.end())
	{
		Atomo at = *it;
		cout << at.intValue() << endl;
		it++;
		};
	};
	
void imprimirActualizacion(Actualizacion &a)
{
	cout << "Actualizacion:" << endl;
	cout << "indice:" << a.i << endl;
	cout << "setdiff:" << endl;
	imprimir_atomos(*a.setDiff);
	};
PlTerm list_from_setAtomo(set<Atomo>* atomos)
{
set<Atomo>::iterator itAtomos = atomos->begin();
	
	if (itAtomos!=atomos->end()) // si tiene atomos
	{
		term_t lst = PL_new_term_ref(); //tuve que usar term_t (C) por que sino no creaba la lista en PlTail
		term_t h = PL_new_term_ref();
		PL_put_nil(lst); 
		//initialize with the empty list
		
		while (itAtomos!=atomos->end())
		{
			Atomo atomo = *itAtomos; //esta declaracion tiene que estar en el bucle para crear bien la lista
			
			h = PL_new_term_ref();
			string str = atomo.strValue();
			PL_put_atom_chars(h, str.c_str());
			PL_cons_list(lst, h, lst);
			
			itAtomos++;
			};
		return *(new PlTerm(lst));
	}
	};

	
	//PlTail lista;	

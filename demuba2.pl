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

dynamic(occurrencia).
dynamic(ractual).

% DEFINICIÓN DE OPERADORES
:-op(0,yfx,-).
%:-op(705,xf, /-).
:-op(700,fy, /-).
:-op(700,xfy,/-).
:-op(550,xfy,:).
:-op(490,yfx,->).
:-op(500,yfx,<->).
:-op(600,yfx,:=).
:-op(600,yfx,=).
:-op(600,xfx,=>).
:-op(400,xfy,v).
:-op(300,xfy,&).
:-op(200,fy,-).
:-op(100,yfx,o).
:-op(100,yfx,y).
:-op(50,xfy,>>).
:-op(20,yfx,/).
:-op(10,yfx,^). 
:-op(200,fy,@).
:-op(200,fy,#).

%PARTE PRINCIPAL. 
%ELIMINE LA POSIBILIDAD DE INCLUIR PREMISAS.

%X /-   :- !, %premisas(K,NP,PA),
%	if_then_else(completart(X,XP,[Xa|XA]), true, (error_sintactico,!,false)),
%	!, XP:XA => Xa:[]:E.

/- Y   :- !, %premisas(K,NP,PA),
	if_then_else(completarf(Y,Yf),true,(error_sintactico,!,false)),
		!, []:[] => Yf:[]:E,!.

X /- Y :- !, 	%premisas(K,NP,PA), 
	if_then_else((completart(X,XP,XA),completarf(Y,Yf)), true, (error_sintactico,!,false)),
		!, XP:XA => Yf:[]:E.


%REGLAS DE SECUENTES + RESOLUCIÓN NO-CLAUSAL

%a la regla siguiente le agregue member en P por que sino hay casos que no funciona.
        
P:PA => [(A=N)]:_:contradiccion(regla_82,A=N) :- opuesto(M,N),(member(A=M,PA); member(A=M,P)),!.

%si es atomico B, lo agrego a la lista de atomos.
P:PA => [(A & B=t)]:Z:strecht(regla_1,A & B=t >> A=t y B=t,T)
	:-  !,([B=t|P]:PA => [(A=t)]:Z:T;  P:PA => [(B=t)]:Z:T).
	
		
P:PA => [(A & B=f)]:Z:split(regla_2,A & B=f >> A=f o B=f,T1,T2)
	:-  !,(P:PA => [(A=f)]:Z:T1, P:PA => [(B=f)]:Z:T2).
	
P:PA => [(A v B=t)]:Z:split(regla_3,A v B=t >> A=t o B=t,T1,T2)
	:-  !,(P:PA => [(A=t)]:Z:T1, P:PA => [(B=t)]:Z:T2).	
	
P:PA => [(A v B=f)]:Z:strecht(regla_4,A v B=f >> A=f y B=f,T)
	:-  (!,[B=f|P]:PA => [(A=f)]:Z:T; !,P:PA => [(B=f)]:Z:T).
	
P:PA => [(- A  =f)]:Z:strecht(regla_5, - A =f >> A=t,T)
	:-  !,P:PA => [(A=t)]:Z:T.
	
P:PA => [(- A  =t)]:Z:strecht(regla_5, - A =t >> A=f,T)
	:-  !,P:PA => [(A=f)]:Z:T.
	
P:PA => [(A -> B=f)]:Z:strecht(regla_6,A -> B=f >> A=t y B=f,T)
	:-  (!,[A=t|P]:PA => [(B=f)]:Z:T; !,P:PA => [(A=t)]:Z:T).	
	
P:PA => [(A -> B=t)]:Z:split(regla_7,A -> B=t >> B=t o A=f,T1,T2)
	:-  !,P:PA => [(B=t)]:Z:T1; !,P:PA => [(A=f)]:Z:T2.
	
P:PA => [(A <-> B=t)]:Z:split(regla_A,A<->B=t >>(A=t y B=t) o (A=f y B=f),T1,T2)
	:-  !,([B=t|P]:PA => [(A=t)]:Z:T1,[B=f|P]:PA => [(A=f)]:Z:T2).
	
P:PA => [(A <-> B=f)]:Z:split(regla_B,A<->B=f>>(A=t y B=f) o (A=f y B=t),T1,T2)
	:-  !,( [A=t|P]:PA => [(B=f)]:Z:T1; P:PA => [(A=t)]:Z:T1),
	!,([B=t|P]:PA => [(A=f)]:Z:T2 ; P:PA => [(B=t)]:Z:T2).
	
P:PA => [(A=N)]:Z:contradiccion(regla_8,A=N)
        :- opuesto(A,N),!.


P:PA => [(A=N)]:Z:replace(regla_9,A=N,EX,FX,T)
	:-
%	 \+member(A=N,PA),
	 !,elegir1(A=N,P,FX,EX,PA),
	 \+member(FX,P),\+member(FX,PA),
%	 delete(P,EX,P2),
	  P:[A=N|PA] => [FX]:Z:T.


%elegir(A,P,R,O,Q,PA):- elegir1(A,P,R,O,PA).
			   	
%elegir1(A,P,NRX1,X1,PA):- 
%premisa(P,PA,PR),promete(PR,A,NX),rs(NX,NRX1).

elegir1(A,[X1|Xs],NRX1,X1,PA):- promete(X1,A,NX1),rs(NX1,NRX1).
elegir1(A,[X1|Xs],Z,Zx,W):-
	elegir1(A,Xs,Z,Zx,W).
	


promete(X=V,Y,NX=V):- prom(X=V,Y,NX).


prom((- X)=VF1,F=VF2,(- Z)):- !,opuesto(VF1,VF1O),prom(X=VF1O,F=VF2,Z).
prom((X1 & X2)=t,F=VF2,Z):-  !,(prom(X1=t,F=VF2,Z);prom(X2=t,F=VF2,Z)).
prom((X1 & X2)=f,F=VF2,W):-  !,(prom(X1=f,F=VF2,Z),W=Z & X2;
				prom(X2=f,F=VF2,Z),W=X1 & Z).
prom((X1 v X2)=t,F=VF2,W):-  !,(prom(X1=t,F=VF2,Z),W=Z v X2;
				prom(X2=t,F=VF2,Z),W=X1 v Z).
prom((X1 v X2)=f,F=VF2,Z):-  !,(prom(X1=f,F=VF2,Z);prom(X2=f,F=VF2,Z)).
prom((X1 -> X2)=t,F=VF2,W):- !,(prom(X2=t,F=VF2,Z),W=X1 -> Z;
				prom(X1=f,F=VF2,Z),W=Z -> X2).
prom((X1 -> X2)=f,F=VF2,Z):-  !,(prom(- X1=f,F=VF2,Z);prom(X2=f,F=VF2,Z)).
prom((X1 <-> X2)=VF1,F=VF2,W):- !,(prom(X2=V,F=VF2,Z),W=X1 <-> Z;
				       prom(X1=V,F=VF2,Z),W=Z <-> X2).


prom(X=VF1,X=VF2,VF2):- !,opuesto(VF1,VF2),!.


%como esta ahora lo haria con un solo atomo.

proatomo(A=VF,P,[FRX1|NP]):-member(F,P), ocurrencia(F,A,PO), reemplaza(F,A=VF,FR),rs(FR=t,FRX1), delete(P,F,NP).

%reemplaza, que hasta ahora es una copia de prom, corta cuando indentifica la primer ocurrencia del atomo
%es necesario que reemplace en todas
%seguramente sea mas eficiente programarlo en C.

reemplaza((- X)=VF1,F=VF2,(- Z)):- !,opuesto(VF1,VF1O),reemplaza(X=VF1O,F=VF2,Z).
reemplaza((X1 & X2)=t,F=VF2,Z):-  !,(reemplaza(X1=t,F=VF2,Z);reemplaza(X2=t,F=VF2,Z)).
reemplaza((X1 & X2)=f,F=VF2,W):-  !,(reemplaza(X1=f,F=VF2,Z),W=Z & X2;
				reemplaza(X2=f,F=VF2,Z),W=X1 & Z).
reemplaza((X1 v X2)=t,F=VF2,W):-  !,(reemplaza(X1=t,F=VF2,Z),W=Z v X2;
				reemplaza(X2=t,F=VF2,Z),W=X1 v Z).
reemplaza((X1 v X2)=f,F=VF2,Z):-  !,(reemplaza(X1=f,F=VF2,Z);reemplaza(X2=f,F=VF2,Z)).
reemplaza((X1 -> X2)=t,F=VF2,W):- !,(reemplaza(X2=t,F=VF2,Z),W=X1 -> Z;
				reemplaza(X1=f,F=VF2,Z),W=Z -> X2).
reemplaza((X1 -> X2)=f,F=VF2,Z):-  !,(reemplaza(- X1=f,F=VF2,Z);reemplaza(X2=f,F=VF2,Z)).
reemplaza((X1 <-> X2)=VF1,F=VF2,W):- !,(reemplaza(X2=V,F=VF2,Z),W=X1 <-> Z;
				       reemplaza(X1=V,F=VF2,Z),W=Z <-> X2).
				       
reemplaza(X=VF1,X=VF2,VF2).			       
				       
r1(A & t,A):- !.
r1(A & f,f):- !.
r1(t & A,A):- !.
r1(f & A,f):- !.
r1(A v t,t):- !.
r1(A v f,A):- !.
r1(t v A,t):- !.
r1(f v A,A):- !.
r1(A -> t,t):- !.
r1(A -> f,- A):- !.
r1(t -> A,A):- !.
r1(f -> A,t):- !.
r1(- t,f):- !.
r1(- f,t):- !.
r1(t <-> A,A):- !.
r1(A <-> t,A):- !.
r1(f <-> A,- A):- !.
r1(A <-> f,- A):- !.
r1(u(X,t),t):- !.
r1(u(X,f),f):- !.
r1(e(X,t),t):- !.
r1(e(X,f),f):- !.
r1(u(X,A),u(X,F)):- r1(A,F),!.
r1(e(X,A),e(X,F)):- r1(A,F),!.
r1(X,X):- !.

r(A & B,R):- !,r(A,RA),r(B,RB),r1(RA & RB,R),!.
r(A v B,R):- !,r(A,RA),r(B,RB),r1(RA v RB,R),!.
r(A -> B,R):- !,r(A,RA),r(B,RB),r1(RA -> RB,R),!.
r(- A,R):- !,r(A,RA),r1(- RA,R),!.
r(X,R):- r1(X,R),!.

rs(A=V,NA=NV):- r(A,X),!,rdp(X=V,NA=NV),!.
rdp(- A = V, Z):- !,opuesto(V,OV),rdp(A = OV,Z).
rdp(X,X).

premisa(P,PA,PR):- member(PR,PA).

opuesto(f,t):- !.
opuesto(t,f):- !. 


%Auxiliar predicates

%premisas(K,[],[]).



if_then_else(Condition, Action1, Action2) :- Condition, !, Action1.  
if_then_else(Condition, Action1, Action2) :- Action2.

error_sintactico :- write('Error sintactico en premisas').

formula(A):- \+ var(A),form(A),!.
form(- A).
form(A & B).
form(A v B).
form(A -> B).
form(A <-> B).
form(u(X,A)).
form(e(X,A)).

atomico(X):- var(X),!.
atomico(A):- \+(formula(A)),!.

completarf(Y1:Ys,[Y1=f|Z]):- !, completarf(Ys,Z).
completarf(Y, [Y=f]).


completart(X1:Xs,PN,[X1=t|Z]):- atomico(X1),!,
				completart(Xs,PN,Z),!.
completart(X1:Xs,[X1=t|W],PA):- !, completart(Xs,W,PA).
completart(X,[],[X=t]):- atomico(X),!.
completart(X,[X=t],[]).


demostrar:- time((read(X), /- X )).
demostrart:- time((read(X), /- X v -X)).

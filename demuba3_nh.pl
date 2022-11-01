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

% Esta versión contiene una nueva codificación del predicado prom.
% La versión original daba como resultado una versión más abreviada
% del resolvente no clausal.
% Ahora en cambio sólo realiza el reemplazo de todos las apariciones
% en la fórmula propuesta, de la variable proposicional del soporte 
% por su correspondiente valor de verdad. 


% DEFINICIÓN DE OPERADORES
:-op(0,yfx,-).
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

/- Y:L   :- !,mkIndex(I), if_then_else(completarf(Y,Yf),true,(error_sintactico,!,false)),!, []:[]:I => Yf:[]:_:L,!.

X /- Y :- !,mkIndex(I), if_then_else((completart(X,XP,XA),completarf(Y,Yf)), true, (error_sintactico,!,false)),!,
	putAtomos(I,XA), XP:XA:I => Yf:[]:_:L.


%REGLAS DE SECUENTES + RESOLUCIÓN NO-CLAUSAL



%si es atomico B, lo agrego a la lista de atomos.
P:PA:I => [(A & B=t)]:Z:strecht(regla_1,A & B=t >> A=t y B=t,T):LS
	:-  !,rdp(B=t,B2=V),if_then_else(atomico(B2),((
	(cpIndex(I,I2),putAtomo(I2,B2=V), call_index_control(I2,P:[B2=V|PA]:I2 => [(A=t)]:Z:T:L1));
	(cpIndex(I,I2),call_index_control(I2,P:PA:I2 => [(B2=V)]:Z:T:L1))),LS is L1+1)
	,
	(((cpIndex(I,I2),indexarF(I2,B2=V),insT(B2=V,P,NP),(call_index_control(I2,NP:PA:I2 => [(A=t)]:Z:T:L2)));
	(cpIndex(I,I2),call_index_control(I2,P:PA:I2 => [(B2=V)]:Z:T:L2))), LS is L2+1)
	).
	
P:PA:I => [(A & B=f)]:Z:split(regla_2,A & B=f >> A=f o B=f,T1,T2):LS
	:-  !,P:PA:I => [(A=f)]:Z:T1:L1, P:PA:I => [(B=f)]:Z:T2:L2, LS is L1+L2+1.
	
P:PA:I => [(A v B=t)]:Z:split(regla_3,A v B=t >> A=t o B=t,T1,T2):LS
	:-  !,P:PA:I => [(A=t)]:Z:T1:L1, P:PA:I => [(B=t)]:Z:T2:L2,LS is L1+L2+1.
	
P:PA:I => [(A v B=f)]:Z:strecht(regla_4,A v B=f >> A=f y B=f,T):LS
	:-  !,rdp(B=f,B2=V),if_then_else(atomico(B2),
	(((cpIndex(I,I2),putAtomo(I2,B2=V), call_index_control(I2,P:[B2=V|PA]:I2 => [(A=f)]:Z:T:L1));
	(cpIndex(I,I2), call_index_control(I2,P:PA:I2 => [(B2=V)]:Z:T:L1))),LS is L1+1)
	,
	(((cpIndex(I,I2),indexarF(I2,B2=V),insT(B2=V,P,NP), call_index_control(I2,NP:PA:I2 => [(A=f)]:Z:T:L2));
	(cpIndex(I,I2), call_index_control(I2,P:PA:I2 => [(B2=V)]:Z:T:L2))),LS is L2+1)
	).
	
	
P:PA:I => [(- A  =f)]:Z:strecht(regla_5, - A =f >> A=t,T):LS
	:-  !,P:PA:I => [(A=t)]:Z:T:L,LS is L+1.
	
P:PA:I => [(- A  =t)]:Z:strecht(regla_5, - A =t >> A=f,T):LS
	:-  !,P:PA:I => [(A=f)]:Z:T:L,LS is L+1.
	
	
_:_:_ => [(A=N)]:_:contradiccion(regla_8,A=N):L
        :- opuesto(A,N),L is 1,!.

P:PA:I => [(A=N)]:_:contradiccion(regla_82,A=N):L :- opuesto(M,N),member(A=M,PA),L is 1,!.

P:PA:I => [(A=N)]:Z:replace(regla_9,A=N,_,FX,T):LS
:- 	 
cpIndex(I,I2),call_index_control(I2,(reduceHs(PA,P,PR,I2),newCache(I2),opuesto(N,M),
mkElegirNh(I2,A=M),putAtomo(I2,A=N),(!,elegir1(I2,A=N,PR,FX),\+(FX=(V=V)),
cpIndex(I2,I3),call_index_control(I3,PR:[A=N|PA]:I3 => [FX]:Z:T:L)))),LS is L+1.

call_index_control(I,CALL):- if_then_else(CALL,(true,rmIndex(I)),(rmIndex(I),!,fail)). 

%elegir1(A,P,XR):- elegir(N),nth1(N,P,X),promete(X,A,NX1),rs(NX1,XR,[A]).
elegir1(I,A,P,XR2):- elegirR(I,N),nth1(N,P,X),rs(X,XR,[A]),
if_then_else(XR=(V=V),elegir1(I,A,P,XR2),XR2=XR).


reduceHs(_,[],[],I).
reduceHs(PA,P,PR,I):- formulasAfectadas(I,FA), dHs(P,PE,FA,0), rHs(I,P,PE,PA,FA,PR).

rHs(I,P,PE,PA,[FI|FA],PR):- nth1(FI,P,F),rs(F,FR,PA),
%write('Atomos F:'),write(FI),imprimirF(FI),
%write('PA:'),write(PA),write('\n'),
%write('F sin reducir:'),write('\n'), write(F),write('\n'),
%write('F reducida:'),write('\n'),write(FR),write('\n'), 
if_then_else(FR=(V=V),eliminarF(I,FI),actualizarF(I,FI,FR)),
%actualizarF(FI,FR),
ins(FR,PE,FI,PRL), rHs(I,P,PRL,PA,FA,PR).

rHs(I,_,PRL,_,[],PRL).

%elimina de P los atomos afectados [f|fa] y el resultado va a PE
dHs(P,PE,[F|FA],O):- N is F+(-O),rem(N,P,PI),NO is O+1,dHs(PI,PE,FA,NO).
dHs(P,P,[],_).


deduccionUnit([P1|P],PA,NP):-P1=(F=_),if_then_else(atomico(F),(append([P1],PAS,PA),deduccionUnit(P,PAS,NP)),(append([P1],NPS,NP),deduccionUnit(P,PA,NPS))).
deduccionUnit([],[],[]).

promete(V=OV,_,V=OV):-opuesto(V,OV),!.

promete(X=V,Y,NX=V):- prom(X=V,Y,NX),!.


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
prom((X1 <-> X2)=_,F=VF2,W):- !,(prom(X2=_,F=VF2,Z),W=X1 <-> Z;
				       prom(X1=_,F=VF2,Z),W=Z <-> X2).


prom(X=VF1,X=VF2,VF2):- !,opuesto(VF1,VF2),!.

opuesto(f,t):- !.
opuesto(t,f):- !.

			       
r1(A & t,A):- !.
r1(_ & f,f):- !.
r1(t & A,A):- !.
r1(f & _,f):- !.
r1(_ v t,t):- !.
r1(A v f,A):- !.
r1(t v _,t):- !.
r1(f v A,A):- !.



r1(_ -> t,t):- !.
r1(A -> f,- A):- !.
r1(t -> A,A):- !.
r1(f -> _,t):- !.

r1(- t,f):- !.
r1(- f,t):- !.
r1(t <-> A,A):- !.
r1(A <-> t,A):- !.
r1(f <-> A,- A):- !.
r1(A <-> f,- A):- !.
r1(u(_,t),t):- !.
r1(u(_,f),f):- !.
r1(e(_,t),t):- !.
r1(e(_,f),f):- !.
r1(u(X,A),u(X,F)):- r1(A,F),!.
r1(e(X,A),e(X,F)):- r1(A,F),!.
r1(X,X):- !.

r(A & B,R,PA):- !,r(A,RA,PA),r(B,RB,PA),r1(RA & RB,R1),(r1(R1,R);R=R1),!.
r(A v B,R,PA):- !,r(A,RA,PA),r(B,RB,PA),r1(RA v RB,R1),(r1(R1,R);R=R1),!.
r(A -> B,R,PA):- !,r(A,RA,PA),r(B,RB,PA),r1(RA -> RB,R1),(r1(R1,R);R=R1),!.
r(A <-> B,R,PA) :- !,r(A,RA,PA),r(B,RB,PA),r1(RA <-> RB,R1),(r1(R1,R);R=R1),!.
r(- A,R,PA):- !,r(A,RA,PA),r1(- RA,R),!.
r(X,V,PA):- member(X=V,PA),!.
r(X,R,_) :- r1(X,R),!.

rs(A=V,NA=NV,PA):- r(A,X,PA),!,rdp(X=V,NA=NV),!.
rdp(- A = V, Z):- !,opuesto(V,OV),rdp(A = OV,Z).
rdp(X,X).

premisa(_,PA,PR):- member(PR,PA).


%Auxiliar predicates


rem(1,[_|L],L):-!.
rem(P,[H|L],LR):-NP is P+(-1),rem(NP,L,LR2),LR=[H|LR2].

len([],0):-!.
len([_|List],L):- len(List,L2), L is L2+1.

ins(Element, List, Position, Result):- nth1(Position, Result, Element, List).
insT(Element, List, Result):- length(List,L), P is L+1, ins(Element,List,P,Result).


%premisas(K,[],[]).

tfin(StartTime,TT):-real_time(EndTime),diftiempo(EndTime,StartTime,TT).
diftiempo(EndTime, StartTime, Time) :-
%	abstime(EndTime, EA),
%	abstime(StartTime, SA),
	Time is '-'(EndTime,StartTime).

abstime(time(Hr,Min,Sec,Hun), Abs) :-
	Abs is Hr*3600 + Min*60 + Sec + Hun/float(100).

if_then_else(Condition, Action1, _) :- Condition, !, Action1.  
if_then_else(_, _, Action2) :- Action2.

error_sintactico :- write('Error sintactico en premisas').

formula(A):- \+ var(A),form(A),!.
form(- _).
form(_ & _).
form(_ v _).
form(_ -> _).
form(_ <-> _).
form(u(_,_)).
form(e(_,_)).

atomico(X):- var(X),!.
atomico(A):- \+(formula(A)),!.

completarf(Y1:Ys,[Y1=f|Z]):- !, completarf(Ys,Z).
completarf(Y, [Y=f]).


completart(X1:Xs,PN,[X1=t|Z]):- atomico(X1),!,
				completart(Xs,PN,Z),!.
completart(X1:Xs,[X1=t|W],PA):- !, registrarF(X1=t),completart(Xs,W,PA).
completart(X,[],[X=t]):- atomico(X),!.
completart(X,[X=t],[]):-registrarF(X=t).

registrarF(F):- indexarF(F).


checkHs([P1|P],PA):- (rs(P1,S,PA),S=(V1=V2),opuesto(V1,V2));checkHs(P,PA).
checkHs([],_):- !,fail.

%checkC(PA):- member(t=f,PA);member(f=t,PA);check(PA).
%checkC(PA):- \+(member(t=f,PA);member(f=t,PA)), check(PA).
%checkC(PA):- \+(member(t=f,PA), member(f=t,PA)), check(PA).

%check([P1|P]):- (P1=(F=V),opuesto(V,OV),member(F=OV,P));check(P).
%check([]):-!,fail.

%check([P1|P]):- P1=(F=V),opuesto(V,OV),\+(member(F=OV,P)),check(P).
%check([]).






getFX([FX|_],_,FX).
getFX([_|LO],LRO,FX):-!,getFX(LO,LRO,FX).

getFX(_,[FX|_],FX).
getFX(LO,[_|LRO],FX):-!,getFX(LO,LRO,FX).

getFX(_,[],_):-!,fail.



generarL(A,[P1|P],L,LR):-generarL(A,P,Lc,Lc2),
if_then_else(promete(P1,A,NP1),(rs(NP1,RP1,[]),append([RP1],Lc,L),Lc2=LR), (append([P1],Lc2,LR),Lc=L)).
generarL(_,[],[],[]).

 elegirH1(A,[X1|Xs],PA):-
 if_then_else(promete(X1,A,NX1),
 (rs(NX1,NRX1,[]),
 %\+(NRX1=(V=V)),
 b_getval(h1r,V),
 heuristica1(NRX1,H1r,V),b_getval(h1f,F),
 if_then_else((F=='';H1r < V), (b_setval(h1r,H1r),b_setval(h1f,NRX1)),true)),
 true), (H1r==0;elegirH1(A,Xs,PA)).
 
 elegirH1(_,[],_).

elegir1(A,[X1|_],NRX1,_,PA):- promete(X1,A,NX1),rs(NX1,NRX1,PA).
elegir1(A,[_|Xs],Z,Zx,W):- elegir1(A,Xs,Z,Zx,W).
 
demostrart_thread(S):- thread_create(demostrart,ID ), thread_join(ID,S).
 
 
demostrartthread:- thread_create( time((read(X), /- X v -X )),ID ), thread_join(ID,S).

demostrar_thread(S):- thread_create(demostrar,ID ), thread_join(ID,S).

demostrart(X,L):- time((read(X), /- X v  -X:L)).
demostrar(X,L):- time((read(X), /- X:L)).

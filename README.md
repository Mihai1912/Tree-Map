createTree: Am alocat memorie pt arbore si am atribuit campurilor arborelui functiile pt crearea unui element, a undei informatii , respectiv distrugerea 
lor , functia ce comparare si root ul acestuia care este NULL si marime 0

isEmpty: Se verifica daca root ul arborelui este NULL daca da se returneaza 1 daca nu 0

search: Se verifica daca valoarea cheii nodului curent este este egala cu valoarea cautata daca nu si este mai mare nodul curent devine copil stang al nodului ,
iar daca este mai mic va deveni copil drept al nodului curent si se repeta procedura pana cand se gaseste elementul dorit

minimum: Se parcurge arborele pana la elementul cel mai din stanga si se returneaza acesta

maximum: Se parcurge arborele pana la elementul cel mai din dreapta si se returneaza acesta

successor: Se verifica daca nodul pt care s-a apelat functia are subarbore are subarbore drept atunci succesorul este minimul acestuia subarbore , 
in caz contrar se va merge din parinte in parinte pana se va gasi succesorul

predecessor: Se verifica daca nodul pt care s-a apelat functia are subarbore stang atunci predecesorul este maximul acestuia subarbore

updateAllHeight: Se parcurge arborele in inordine si se actualizeaza inlatimea pt fiecare nod in parte

avlRotateLeft: Se verifica daca nodul pt care s-a apelat functia este root daca da subarborele stang al copilului din dreapta nodului pentru care
s-a apelat functia devine copil de dreapta pentru nodul care s-a apelat functia se seteaza parintele acestuia nodul petnru care s-a apelat fuctia 
devine copil de stanga al copilului sau de dreapta , parintele acestuia devin null pentru ca este root acum, se actualizeaza inaltimile si se 
seteaza rootul ca fiind copilul de dreapta al nodului pentru care s-a apelat functia , iar in cazul in care nodul pentru care s-a apelat functia 
nu este root se procedeaza similar doar ca se leaga copilul de dreapta al nodului de parintele nodului (se leaga de restul arborelui)

avlRotateRight: Se verifica daca nodul pt care s-a apelat functia este root daca da subarborele drept al copilului din stanga nodului pentru care
s-a apelat functia devine copil de stang pentru nodul care s-a apelat functia se seteaza parintele acestuia nodul petnru care s-a apelat fuctia 
devine copil de dreapta al copilului sau de stanga , parintele acestuia devin null pentru ca este root acum, se actualizeaza inaltimile si se 
seteaza rootul ca fiind copilul de stanga al nodului pentru care s-a apelat functia , iar in cazul in care nodul pentru care s-a apelat functia 
nu este root se procedeaza similar doar ca se leaga copilul de dreapta al nodului de parintele nodului (se leaga de restul arborelui)

avlGetBalance: Se calculeaza factorul de echilibru al unui nod scazand inaltimea nodului din dreapta din inaltimea nodului din stanga

avlFixUp: In functia avlFixUp se verifica toate nodurile de la nodul de la care s-a apelat functia pana la root ul arborelui si se verifica daca acesta este
ezechilibrat , daca balancefactorul este mai mic decat -1 se ca efectua o rotire la stanga , daca acesta este mai mare ca 1 se va efectua o rotire la dreapta , 
daca balancefactorul este mai mica decat -1 si exista copilul stang al maximului se vor face o rotatie la dreapta pt maximul arborelui si o rotire la stanga pt
nodul curent.

setPrevNext: Se parcurge arborele in inordine si se seteaza campurile next si prev ale unui nod folosind functiile successor si predecessor

insert: Daca arborele este gol se creaza un element si este setat ca root , iar daca nu se parcurge arborele pana la parintele nodului de inserat 
se verifica daca acesta este mai mare daca da se insereaza in dreapta parintelui daca nu se inseraza in stanga , iar daca dorim sa inseram un node 
cu aceeazi cheie se va crea o lista de duplicate si se insereaza in aceasta

delete: Se verifica daca nodul de sters este un nod din arbore sau daca este un nod din lista , daca este nod din arbore se verifica daca nodul este frunza daca
da se sterge simpul si se apeleaza functia de fix , apoi este cazul in care nodul de sters este root si se cauta minimul din subarborele drept si se ilocuieste 
cu rootul , iar apoi se verifica daca nodul de sters are un copil pe stanga sau pe dreapta , daca da coilul nodului de sters se leaga la parintele nodului de 
sters , iar apoi este cazul in care nodul de sters face parte din lista de duplicate daca da se verifica daca lista de duplicate este lista ultimului element 
din arbore (maximul adica nu are end->next) , daca acesta nu este in lista de diplicate a maximului se sterge nodul si se actualizeaza lagaturile intre nextul
nodului de sters si prevul acestuia dupa care se sterge nodul , iar daca nodul se sters este in lista ultimului element din arbore se sterge simplu iar end ul
listei de duplicate devine chiar prevul nodului sters.
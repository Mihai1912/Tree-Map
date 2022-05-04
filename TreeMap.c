#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreeMap.h"

#define MAX(a, b) (((a) >= (b))?(a):(b))


/* Creeaza un arbore cu o serie de metode asociate
 *
 * return: arborele creat
 */
TTree* createTree(void* (*createElement)(void*),
				  void (*destroyElement)(void*),
				  void* (*createInfo)(void*),
				  void (*destroyInfo)(void*),
				  int compare(void*, void*)) {
	TTree* tree = (TTree*)malloc(sizeof(TTree));
	tree->createElement = createElement;
	tree->createInfo = createInfo;
	tree->destroyElement = destroyElement;
	tree->destroyInfo = destroyInfo;
	tree->compare = compare;
	tree->root = NULL;
	tree->size = 0;
	return tree;
}


/* Verifica daca un arbore este gol (vid)
 * 		1 - daca arborele este gol
 * 		0 - in caz contrar
 */
int isEmpty(TTree* tree) {
	if (tree->root == NULL) {
		return 1;
	} else {
		return 0;
	}
}


/* Cauta un anumit element in interiorul unui arbore
 *
 * tree: structura cu metodele asociate arborelui
 *   !folosit pentru a putea compara elementul curent cu cel cautat
 *
 * x: radacina arborelui curent (in care se face cautarea)
 * elem: elementul ce trebuie cautat
 */
TreeNode* search(TTree* tree, TreeNode* x, void* elem) {
	TreeNode* node = x;
	while (node != NULL) 
	{
		if (tree->compare(node->elem , elem) == 1) {
			node = node->left;
		} else if (tree->compare(node->elem , elem) == -1) {
			node = node->right;
		} else if (tree->compare(node->elem , elem) == 0) {
			break;
		}
	}
	return node;
}


/* Gaseste nodul cu elementul minim dintr-un arbore
 * avand radacina in x
 */
TreeNode* minimum(TreeNode* x) {
	TreeNode* node = x;
	if (x != NULL) {
		while (node->left != NULL)
		{	
			node = node->left;
		}
	}
	return node;
}

/* Gaseste nodul cu elementul maxim dintr-un arbore
 * avand radacina in x
 */
TreeNode* maximum(TreeNode* x) {
	TreeNode* node = x;
	if (x != NULL) {
		while (node->right != NULL)
		{	
			node = node->right;
		}
	}
	return node;
}


/* Functie pentru gasirea succesorului unui nod
 * (succesorul in inordine)
 */
TreeNode* successor(TreeNode* x) {
	TreeNode* node = x , *p=x->parent;
	// daca nodul pt care dorim sa aflam succesorul are subarbore drept atunci succesorul este minimul acestuia subarbore
	if (node->right != NULL) {
		return minimum(node->right);
	} 
	// in caz contrar se va merge din parinte in parinte pana se va gasi succesorul
	else {
		while (p != NULL && node == p->right)
		{
			node = p;
			p = p->parent;
		}
		return p;
	}
	return NULL;
}


/* Functie pentru gasirea predecesorului unui nod
 * (predecesorul in inordine)
 */
TreeNode* predecessor(TreeNode* x) {
	TreeNode* node = x , *p=x->parent;
	// daca nodul pt care dorim sa aflam succesorul are subarbore stang atunci predecesorul este maximul acestuia subarbore
	if (node->left != NULL) {
		return maximum(node->left);
	} 
	// in caz contrar se va merge din parinte in parinte pana se va gasi predecesor
	else {
		while (p != NULL && node == p->left)
		{
			node = p;
			p = p->parent;
		}
		return p;
	}
	return NULL;
}


/* Actualizeaza inaltimea unui nod din arbore
 */
void updateHeight(TreeNode* x) {

	int leftHeight = 0;
	int rightHeight = 0;

	if (x != NULL) {
		if (x->left != NULL)  leftHeight  = x->left->height;
		if (x->right != NULL) rightHeight = x->right->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}

/* Actualizeaza inaltimea tuturor nodurilor din arbore
 */

void updateAllHeight(TreeNode* node) {
	// se parcurge arborele in inordine si se actualizeaza inlatimea pt fiecare nod in parte
	if (node->left != NULL) updateAllHeight(node->left);
	if (node->right != NULL) updateAllHeight( node->right);
	updateHeight(node);
	return;
}


/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la stanga
 * a subarborelui care are varful in x
 *
 *   (x)	 Rotatie    (y)
 *	 / \     stanga     / \
 *  a  (y)    ---->   (x)  c
 *	   / \			  / \
 *    b   c          a   b
 */
void avlRotateLeft(TTree* tree, TreeNode* x) {
	if (tree == NULL || x == NULL) return;
	// cazul in care nodul pentru care s-a apelat rotirea este root-ul arborelui
	if (x == tree->root) {
		TreeNode *aux = x->right;
		x->right = aux->left;
		if (aux->left != NULL) aux->left->parent = x;
		aux->left = x;
		x->parent = aux;
		aux->parent = NULL;
		updateHeight(x);
		updateHeight(aux);
		tree->root = aux;
		return;
	} 
	// cazul in care nodul pentru care s-a apelat rotirea nu este root-ul arborelui si trebuie legat de restul arborelui
	else {
		TreeNode *aux , *parinte;
		parinte = x->parent;
		int ind_dir;
		if (parinte->right != NULL && parinte->right == x) {
			ind_dir = 1;
		}
		if (parinte->left != NULL && parinte->left == x) {
			ind_dir = 0;
		}
		aux = x->right;
		x->right = aux->left;
		if (aux->left != NULL) aux->left->parent = x;
		aux->left = x;
		x->parent = aux;
		aux->parent = parinte;
		if (ind_dir == 1) {
			parinte->right = aux;
		} else {
			parinte->left = aux;
		}
		return;
	}
}


/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la dreapta
 * a subarborelui care are varful in y
 *
 *     (y)	 Rotatie    (x)
 *	   / \   dreapta    / \
 *   (x)  c   ---->    a  (y)
 *	 / \			      / \
 *  a   b                b   c
 */
void avlRotateRight(TTree* tree, TreeNode* y) {
	if (tree == NULL || y == NULL) return;
	// cazul in care nodul pentru care s-a apelat rotirea este root-ul arborelui
	if (y == tree->root) {
		TreeNode *aux = y->left;
		y->left = aux->right;
		aux->right = y;
		y->parent = aux;
		aux->parent = NULL;
		updateHeight(y);
		updateHeight(aux);
		tree->root = aux;
		return;
	} 
	// cazul in care nodul pentru care s-a apelat rotirea nu este root-ul arborelui si trebuie legat de restul arborelui
	else {
		TreeNode *aux , *parinte;
		parinte = y->parent;
		int ind_dir;
		if (parinte->right != NULL && parinte->right == y) {
			ind_dir = 1;
		} 
		if (parinte->left != NULL && parinte->left == y) {
			ind_dir = 0;
		}
		aux = y->left;
		y->left = aux->right;
		if (aux->right != NULL) aux->right->parent = y;
		aux->right = y;
		y->parent = aux;
		aux->parent = parinte;
		if (ind_dir == 1) {
			parinte->right = aux;
		} else {
			parinte->left = aux;
		}
		return;
	}
}

/* Calculeaza factorul de echilibrare pentru un nod x
 * (AVL balance factor)
 */
int avlGetBalance(TreeNode *x) {
	if (x == NULL)
		return 0;
	if (x->right != NULL && x->left != NULL) {
		return ((x->left->height) - (x->right->height));
	} 
	if (x->right == NULL && x->left != NULL) {
		return ((x->left->height));
	} 
	if (x->right != NULL && x->left == NULL) {
		return (0 - (x->right->height));
	}
	return 0;
}


/* Functie pentru reechilibrarea unui arbore AVL
 * in urma unei inserari prin rotatii simple sau duble
 * si recalcularea inaltimii fiecarui nod intalnit parcurgand
 * arborele de jos in sus, spre radacina
 *
 */
void avlFixUp(TTree* tree, TreeNode* y) {
			TreeNode *aux = y;
			int balanceFact = avlGetBalance(aux);
			while (1)
			{
				if (balanceFact < -1 && maximum(tree->root)->left != NULL) {
					avlRotateRight(tree , maximum(tree->root));
					avlRotateLeft(tree , aux);
					updateAllHeight(tree->root);
					return;
				}
				if (balanceFact < -1) {
					avlRotateLeft(tree , aux);
					updateAllHeight(tree->root);
					return;
				}	
				if (balanceFact > 1) {
					avlRotateRight(tree , aux);
					updateAllHeight(tree->root);
					return;
				}
				if (aux->parent == NULL) break;
				aux = aux->parent;
				balanceFact = avlGetBalance(aux);
				updateAllHeight(tree->root);
			}
}


/* Functie pentru crearea unui nod
 *
 * value: valoarea/cheia din cadrul arborelui
 * info: informatia/valoarea din dictionar
 */
TreeNode* createTreeNode(TTree *tree, void* value, void* info) {

	if (tree == NULL)
		return NULL;

	// Alocarea memoriei
	TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));

	// Setarea elementului si a informatiei
	node->elem = tree->createElement(value);
	node->info = tree->createInfo(info);

	// Initializarea legaturilor din cadrul arborelui
	node->parent = node->right = node->left = NULL;

	// Initializarea legaturilor din cadrul listei dublu inlantuite
	node->next = node->prev = node->end = NULL;


	// Inaltimea unui nod nou este 1
	// Inaltimea lui NULL este 0
	node->height = 1;

	return node;
}

/* Setarea pentru fiecare nod a campului next si prev
 */

void setPrevNext(TTree* tree, TreeNode* node) {
	// parcurgerea arborelui in inordine si setarea campului next sau prev cu ajutorul functiilor successor si predecessor
    if (node->left != NULL) setPrevNext(tree , node->left);
    node->next = successor(node);
    node->prev = predecessor(node);
    if (node->right != NULL) setPrevNext(tree , node->right);
    return;
}

/* Inserarea unul nou nod in cadrul multi-dictionarului
 * ! In urma adaugarii arborele trebuie sa fie echilibrat
 *
 */


void insert(TTree* tree, void* elem, void* info) {
	TreeNode* node , *aux;
	node = createTreeNode(tree , elem , info);
	if (tree == NULL) return;
	if (tree != NULL) {
		if (tree->root == NULL) {
			tree->root = node;
			tree->root->end = tree->root;
			tree->size++;
			return;
		}
		else {
			aux = tree->root;
			// parcurgere pana la parintele nodului de inserat
			while (aux != NULL && (aux->right != NULL || aux->left != NULL))
			{
				if (tree->compare(aux->elem , node->elem) == 1) {
					aux = aux->left;
				} else if (tree->compare(aux->elem , node->elem) == -1){
					aux = aux->right;
				} else if (tree->compare(aux->elem , node->elem) == 0) {
					break;
				}
			}
			// inserare la stanga parintelui
			if (aux != NULL && tree->compare(aux->elem , node->elem) == 1) {
				aux->left = node;
				node->parent = aux;
				node->end = node;
				updateHeight(aux);
				setPrevNext(tree , tree->root);
				avlFixUp(tree , node);
				updateAllHeight(tree->root);
			} 
			// inserare la dreapata parintelui
			else if (aux != NULL && tree->compare(aux->elem , node->elem) == -1) {
				aux->right = node;
				node->parent = aux;
				node->end = node;
				updateHeight(aux);
				setPrevNext(tree , tree->root);
				avlFixUp(tree , node);
				updateAllHeight(tree->root);
			} 
			// inserare in lista de duplicate
			else if (aux != NULL && tree->compare(aux->elem , node->elem) == 0) {
				// cazul in care in lista de duplicate nu este lista celui mai mare nr din arbore
				if (aux->next != NULL) {
					// cazul in care lista de duplicate este goala
					if (aux != aux->end) {
						node->next = aux->end->next;
						aux->end->next->prev = node;
						node->prev = aux->end;
						aux->end->next = node;
						aux->end = node;
					}
					// cazul in care lista de duplicate nu este goala
					if (aux == aux->end) {
						node->next = aux->next;
						aux->next->prev = node;
						aux->end = node;
						aux->next = aux->end;
						node->prev = aux;
					}
				} 
				// cazul in care in lista de duplicate este lista celui mai mare nr din arbore
				else {
					node->prev = aux->end;
					aux->end->next = node;
					aux->end = node;
				}
			}
		}
		tree->size++;
		return;
	}
}


/* Eliminarea unui nod dintr-un arbore
 *
 * ! tree trebuie folosit pentru eliberarea
 *   campurilor `elem` si `info`
 */
void destroyTreeNode(TTree *tree, TreeNode* node){

	// Verificarea argumentelor functiei
	if(tree == NULL || node == NULL) return;

	// Folosirea metodelor arborelui
	// pentru de-alocarea campurilor nodului
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// Eliberarea memoriei nodului
	free(node);
}


/* Eliminarea unui nod din arbore
 *
 * elem: cheia nodului ce trebuie sters
 * 	! In cazul in care exista chei duplicate
 *	  se va sterge ultimul nod din lista de duplicate
 */
void delete(TTree* tree, void* elem) {
	TreeNode *del_node = search(tree , tree->root , elem) , *parinte , *aux , *aux2;
	aux2 = del_node;
	// cazul in care dorim sa stergem un nod din arbore
	if (del_node == del_node->end) {
		parinte = del_node->parent;
		// cazul in care nodul de sters este frunza 
		if (del_node->right == NULL && del_node->left == NULL && del_node != tree->root) {
			parinte = del_node->parent;
			if (del_node->parent->right == del_node) {
				del_node->parent->right = NULL;
			}
			if (del_node->parent->left == del_node) {
				del_node->parent->left = NULL;
			}
			destroyTreeNode(tree , del_node);
			avlFixUp(tree , parinte);
			tree->size--;
			setPrevNext(tree , tree->root);
			return;
		}
		// cazul in care nodul de sters este root
		if (del_node == tree->root) {
			// cazul in care in arbore se afla decat root ul
			if (del_node->left == NULL && del_node->right == NULL) {
				tree->root = NULL;
				destroyTreeNode(tree , del_node);
				tree->size--;
				return;
			}
			// cazul in care in arbore nu se afla decat root ul
			aux = minimum(del_node->right);
			if (aux->right != NULL) {
				if (aux->parent->left == aux) {
					aux->parent->left = aux->right;
				}
				if (aux->parent->right == aux) {
					aux->parent->right = aux->right;
				}
			}
			aux->left = del_node->left;
			aux->right = del_node->right;
			del_node->right->parent = aux;
			del_node->left->parent = aux;
			if (aux->parent->left == aux) {
				aux->parent->left = NULL;
			}
			if (aux->parent->right == aux) {
				aux->parent->right = NULL;
			}
			aux->parent = NULL;
			tree->root = aux;
			destroyTreeNode(tree , del_node);
			tree->size--;
			setPrevNext(tree , tree->root);
			avlFixUp(tree , maximum(tree->root));
			return;
		}

		// cazul in care nodul de sters are decat copil de dreapta
		if (del_node->right != NULL && del_node->left == NULL) {
			aux = del_node->right;
			aux->parent = del_node->parent;
			if (del_node->parent->right == del_node) {
				del_node->parent->right = aux;
			}
			if (del_node->parent->left == del_node) {
				del_node->parent->left = aux;
			}
			destroyTreeNode(tree , del_node);
			tree->size--;
			setPrevNext(tree , tree->root);
			return;
		}

		// cazul in care nodul de sters are decat copil de stanga
		if (del_node->left != NULL && del_node->right == NULL) {
			aux = del_node->left;
			aux->parent = del_node->parent;
			if (del_node->parent->left == del_node) {
				del_node->parent->left = aux;
			}
			if (del_node->parent->right == del_node) {
				del_node->parent->right = aux;
			}
			destroyTreeNode(tree , del_node);
			tree->size--;
			setPrevNext(tree , tree->root);
			return;
		}
	}

	// cazul in care dorim sa stergem un nod din lista de duplicate
	if (del_node != del_node->end) {
		// cazul in care nodul de sters este in lista unui nod din arbore care are predecesor
		if (del_node->end->next != NULL) {
			del_node = del_node->end;
			aux = del_node->prev;
			aux->next = del_node->next;
			del_node->next->prev = aux;
			aux2->end = aux;
			destroyTreeNode(tree , del_node);
			return;
		} 
		// cazul in care nodul de sters este in lista unui nod care nu are predecesor
		else {
			del_node = del_node->end;
			aux = del_node->prev;
			aux->next = NULL;
			aux2->end = aux;
			destroyTreeNode(tree , del_node);
			return;
		}
	}
}


/* Eliberarea memoriei unui arbore
 */
void destroyTree(TTree* tree){

	/* Se poate folosi lista dublu intalntuita
	 * pentru eliberarea memoriei
	 */
	if (tree == NULL || tree->root == NULL)
		return;
	TreeNode *aux = maximum(tree->root) , *aux2 = aux;
	while (aux != NULL) {
			aux2 = aux;
			aux=aux->prev;
			destroyTreeNode(tree , aux2);
	}
	free(tree);
}
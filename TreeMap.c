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
		if (tree->compare(node->info , elem) == 1) {
			node = node->left;
		} else if (tree->compare(node->info , elem) == -1) {
			node = node->right;
		} else if (tree->compare(node->info , elem) == 0) {
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
	if (node->right != NULL) {
		return minimum(node->right);
	} else {
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
	if (node->left != NULL) {
		return maximum(node->left);
	} else {
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



void updateAllHeight(TreeNode* node) {
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
	} else {
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
	} else {
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
		return ((x->right->height) - (x->left->height));
	} 
	if (x->right == NULL && x->left != NULL) {
		return (0 - (x->left->height));
	} 
	if (x->right != NULL && x->left == NULL) {
		return ((x->right->height));
	}
}


/* Functie pentru reechilibrarea unui arbore AVL
 * in urma unei inserari prin rotatii simple sau duble
 * si recalcularea inaltimii fiecarui nod intalnit parcurgand
 * arborele de jos in sus, spre radacina
 *
 */
void avlFixUp(TTree* tree, TreeNode* y) {
	TreeNode* node = y, * min , * max;
	TreeNode* tmp = tree->root;
	min = minimum(tree->root);
	max = maximum(tree->root);

	while ((avlGetBalance(tmp) > 1 || avlGetBalance(tmp) < -1))
	{
		if (avlGetBalance(tmp->right) > 1) {
			tmp = tmp->right;
			continue;
		}
		if (avlGetBalance(tmp->left) > 1) {
			tmp = tmp->left;
			continue;
		}
		if (avlGetBalance(tmp->right) < -1) {
			tmp = tmp->right;
			continue;
		}
		if (avlGetBalance(tmp->left) < -1) {
			tmp = tmp->left;
			continue;
		}
		if ((avlGetBalance(tmp->right) == 1 || avlGetBalance(tmp->right) == -1 || avlGetBalance(tmp->right) == 0) &&
		    (avlGetBalance(tmp->left) == 1 || avlGetBalance(tmp->left) == -1 || avlGetBalance(tmp->left) == 0)) {
				break;
			}
	}
		

	if (node == max->left){
		avlRotateRight(tree , max);
		avlRotateLeft(tree , max->parent->parent);
		return;
	}
	if (node == min->right){
		avlRotateLeft(tree , min);
		avlRotateRight(tree , min->parent);
		return;	
	}


	if (avlGetBalance(tree->root) != 1 || avlGetBalance(tree->root) != -1 || avlGetBalance(tree->root) != 0) {
			if (avlGetBalance(tmp) > 1) {
				avlRotateLeft(tree , tmp);
			}
			updateAllHeight(tree->root);
			if (avlGetBalance(tmp) < -1) {
			avlRotateRight(tree , tmp);
			}
			updateAllHeight(tree->root);
	}


	if (avlGetBalance(tree->root) == 1 || avlGetBalance(tree->root) == -1 || avlGetBalance(tree->root) == 0) {
		while (max->parent != NULL) {
			if (avlGetBalance(max) > 1) {
				avlRotateLeft(tree , max);
				break;
			}
			if (avlGetBalance(max) < -1) {
				avlRotateRight(tree , max);
				break;
			}
			max = max->parent;
		}
		while (min->parent != NULL) {
			if (avlGetBalance(min) > 1) {
				avlRotateLeft(tree , min);
				break;
			}
			if (avlGetBalance(min) < -1) {
				avlRotateRight(tree , min);
				break;
			}
			min = min->parent;
		}
	}
	return;
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


/* Inserarea unul nou nod in cadrul multi-dictionarului
 * ! In urma adaugarii arborele trebuie sa fie echilibrat
 *
 */

void setPrevNext(TTree* tree, TreeNode* node) {
	if (node->left != NULL) setPrevNext(tree , node->left);
	if (node != maximum(tree->root)){
		node->next = successor(node);
	} else if (node == maximum(tree->root)) {
		node->next = NULL;
	}
	if (node != minimum(tree->root)) {
		node->prev = predecessor(node);
	} else if (node == minimum(tree->root)) {
		node->prev = NULL;
	}
	if (node->right != NULL) setPrevNext(tree , node->right);
	return;
}


void insert(TTree* tree, void* elem, void* info) {
	printf("==========\n");
	TreeNode* node , * aux , *a , *b , *c;
	if (tree == NULL) return;
	if (tree != NULL) {
		if (tree->root == NULL) {
			tree->root = createTreeNode(tree , elem , info);
			tree->root->end = tree->root;
			tree->size++;
			return;
		} 
		else {
			node = createTreeNode(tree , elem , info);
			aux = tree->root;
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
			aux->end = aux;
			aux->end->prev = aux;
			aux->next = aux->end;
			if (tree->compare(aux->elem , node->elem) == 1) {
				aux->left = node;
				node->parent = aux;
				node->end = node;
				updateHeight(aux);
				setPrevNext(tree , tree->root);
			} else if (tree->compare(aux->elem , node->elem) == -1) {
				aux->right = node;
				node->parent = aux;
				node->end = node;
				updateHeight(aux);
				setPrevNext(tree , tree->root);
			} else if (tree->compare(aux->elem , node->elem) == 0) {
				printf("valoare elemnet aux = %d\n" , *(int *)(aux->elem));
				printf("valoare elemnet node = %d\n" , *(int *)(node->elem));
				if (aux == aux->end) {
					printf("ok!\n");
					if (aux->end->next != NULL) {
						printf("---------\n");
						printf("%p\n" , aux);
						printf("%p\n" , aux->end);
						printf("%p\n" , aux->end->prev);
						printf("%p\n" , aux->next);
						aux->next->prev = node;
						node->next = aux->next;
						aux->end = node;
						aux->next = node;
						node->prev = aux;
						printf("-%p\n" , aux);
						printf("-%p\n" , aux->end);
						printf("-%p\n" , aux->end->prev);
						printf("-%p" , aux->next);
					} else {
						aux->end->next = node;
						node->prev = aux->end;
						aux->end = node;
					}
				} else {
					printf("alooo\n");
				}
			}
		}
		updateAllHeight(tree->root);
		avlFixUp(tree , node);
		
		setPrevNext(tree , tree->root);

		a = minimum(tree->root);
		b = maximum(tree->root);
		c = a;


		printf("\n");
		while (a != NULL)
		{
			printf("%d " , *(int *)(a->elem));
			a=a->next;
		}
		printf("\n");
		while (b != NULL)
		{
			printf("%d " , *(int *)(b->elem));
			b=b->prev;
		}
		printf("\n");

		while (c != NULL)
		{
			printf("end=%d/%d " , *(int *)(c->end->elem) , *(int *)(c->end->info));
			c=c->next;
		}
		printf("\n");

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
	return;
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
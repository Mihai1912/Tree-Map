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
	TreeNode* node = x;
	if (node->right != NULL) {
		return minimum(node->right);
	} else {
		while (node->parent != NULL && node != node->parent->left)
		{
			node = node->parent;
		}
		return node->parent;
	}
}


/* Functie pentru gasirea predecesorului unui nod
 * (predecesorul in inordine)
 */
TreeNode* predecessor(TreeNode* x) {
	TreeNode* node = x;
	if (node->left != NULL) {
        return maximum(node->left);
    } else {
        while (node->parent != NULL && node != node->parent->right) {
            node = node->parent;
        }
        return node->parent;
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
		aux->left = x;
		x->parent = aux;
		aux->parent = NULL;
		updateHeight(x);
		updateHeight(aux);
		tree->root = aux;
		return;
	} else {
		TreeNode *aux = x->right;
		x->right = aux->left;
		aux->left = x;
		updateHeight(x);
		updateHeight(aux);
		TreeNode *parinte;
		int dir_ind;
		parinte = x->parent;
		if (parinte->left == x) {
			dir_ind = 1;
		} else if (parinte->right == x){
			dir_ind = 0;
		}
		x = aux;
		if (dir_ind == 1) {
			parinte->left = x;
		} else if(dir_ind == 0){
			parinte->right = x;
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
		TreeNode *aux = y->left;
		TreeNode *parinte;
		int dir_ind;
		parinte = y->parent;
		if (parinte->left != NULL && parinte->left == y) {
			dir_ind = 1;
		}
		if (parinte->right != NULL && parinte->right == y) {
			dir_ind = 0;
		}
		y->left = aux->right;
		aux->right = y;
		updateHeight(y);
		updateHeight(aux);
		y = aux;
		if (dir_ind == 1) {
			parinte->left = y;
		} else {
			parinte->right = y;
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
		avlRotateLeft(tree , max->parent);
	}
	if (node == min->right){
		avlRotateLeft(tree , min);
		avlRotateRight(tree , min->parent);	
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
		while (min->parent != NULL) {
			if (avlGetBalance(min) > 1) {
				avlRotateLeft(tree , min);
			}
			if (avlGetBalance(min) < -1) {
				avlRotateRight(tree , min);
			}
			min = min->parent;
		}
		while (max->parent != NULL) {
			if (avlGetBalance(max) > 1) {
				avlRotateLeft(tree , max);
			}
			if (avlGetBalance(max) < -1) {
					avlRotateRight(tree , max);
			}
			max = max->parent;
		}
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


/* Inserarea unul nou nod in cadrul multi-dictionarului
 * ! In urma adaugarii arborele trebuie sa fie echilibrat
 *
 */

void insert(TTree* tree, void* elem, void* info) {
	TreeNode* node , * aux , *min , *max , *tmp_min , *tmp_max;
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
			if (tree->compare(aux->elem , node->elem) == 1) {
				aux->left = node;
				node->parent = aux;
				node->end = node;
				updateHeight(aux);
			} else if (tree->compare(aux->elem , node->elem) == -1) {
				aux->right = node;
				node->parent = aux;
				node->end = node;
				updateHeight(aux);
			} else if (tree->compare(aux->elem , node->elem) == 0) {
				if (aux->next == NULL) {
					aux->next = node;
					node->prev = aux;
					aux->end = node;
					aux->end->next = NULL;	
				}
			}
		}
		updateAllHeight(tree->root);
		avlFixUp(tree , node);
		min = minimum(tree->root);
		max = maximum(tree->root);

		tmp_max = max;
		tmp_min = min;

		while (successor(tmp_min) != NULL)
		{
			tmp_min->next = successor(tmp_min);
			tmp_min = successor(tmp_min);
		}

		while (predecessor(tmp_max) != NULL)
		{
			tmp_max->prev = predecessor(tmp_max);
			tmp_max = predecessor(tmp_max);
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
	return;
}


/* Eliberarea memoriei unui arbore
 */
void destroyTree(TTree* tree){

	/* Se poate folosi lista dublu intalntuita
	 * pentru eliberarea memoriei
	 */
	free(tree);
	if (tree == NULL || tree->root == NULL)
		return;
}
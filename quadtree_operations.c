#include "quadtree_operations.h"

void init_node(QuadtreeNode **node) {

	/*initializeaza cu 0 memoria unui nod din arbore*/

	*node = calloc(1,sizeof(QuadtreeNode));
	
	(*node)->is_empty = 1;

}

void insert_into_tree(QuadtreeNode **root, pixel *matrix, int area) {

	/*functie recursiva ce formeaza arborele asociat unei matrice de pixeli*/

	init_node(root);

	(*root)->area = area;

	if (has_same_color(matrix, area)) {

		(*root)->blue = matrix[0].b;
		(*root)->green = matrix[0].g;
		(*root)->red = matrix[0].r;
		(*root)->reserved = matrix[0].res;

		(*root)->is_empty = 0;

		free(matrix);

		return;

	} else {

		(*root)->is_empty = 1;

		insert_into_tree(&(*root)->p1, get_top_l(matrix, (int)sqrt(area), (int)sqrt(area)), area / 4);
		insert_into_tree(&(*root)->p2, get_top_r(matrix, (int)sqrt(area), (int)sqrt(area)), area / 4);
		insert_into_tree(&(*root)->p3, get_bot_r(matrix, (int)sqrt(area), (int)sqrt(area)), area / 4);
		insert_into_tree(&(*root)->p4, get_bot_l(matrix, (int)sqrt(area), (int)sqrt(area)), area / 4);

		free(matrix);
	}

}

void add_node(QuadtreeNode **node, QuadtreeNode_compressed *buffer) {

	/*reconstruieste un nod din arbore dupa forma comprimata a acestuia*/

	init_node(node);

	(*node)->area = buffer->area;
	(*node)->blue = buffer->blue;
	(*node)->green = buffer->green;
	(*node)->red = buffer->red;
	(*node)->reserved = buffer->reserved;
	(*node)->is_empty = 0;
	(*node)->top_left = buffer->top_left;
	(*node)->top_right = buffer->top_right;
	(*node)->bottom_right = buffer->bottom_right;
	(*node)->bottom_left = buffer->bottom_left;

}

ListNode *ultim;

void enqueue(ListNode **head, QuadtreeNode *val) {

	/*adauga un nod din arbore in coada*/

	ListNode *nou = malloc(sizeof(ListNode));
	
	nou->val = val;
	nou->next = NULL;

	if (*head == NULL) {

		*head = nou;
		(*head)->next = NULL;
		ultim = *head;

		return;
	
	}

	if (ultim) {
	
		ultim->next = nou;
		ultim = ultim->next;
	
	}
}

ListNode *dequeue(ListNode **head) {

	/*extrage un nod din coada*/
	/*memoria ocupata de acesta va trebui eliberata ulterior*/

	ListNode *tmp;

	if (!(*head)) {
		
		printf("Eroare : dequeue pe o stiva goala!");
		return NULL;
	
	}

	if ((*head)->next == NULL) {
		
		tmp = *head;
		*head = NULL;
		ultim = NULL;
	
		return tmp;
	
	}


	tmp = *head;
	*head = (*head)->next;
	
	return tmp;

}

void build_index(int *current_index, QuadtreeNode *not_indexed){
	
	/*construieste indecsii unui nod din arbore*/
	/*va fi apelata de functia bfs*/

	if (not_indexed->is_empty) {

		/*daca este nod parinte*/

		not_indexed->top_left = (*current_index)++;
		not_indexed->top_right = (*current_index)++;
		not_indexed->bottom_right = (*current_index)++;
		not_indexed->bottom_left = (*current_index)++;
	
	} else {

		/*daca este frunza*/

		not_indexed->top_left = -1;
		not_indexed->top_right = -1;
		not_indexed->bottom_right = -1;
		not_indexed->bottom_left = -1;
	
	}

}


void bfs(QuadtreeNode *root, QuadtreeNode_compressed *vector) {

	/*parcurge arborele in mod BFS, construieste indecsii fiecarui nod,
	apoi il introduce pe acesta in vectorul de noduri comprimate*/

	ListNode *q, *tmp;

	/*initializare*/
	
	q = NULL;

	enqueue(&q, root);

	int index = 1;

	int current_node = 0;

	while (q) {

		tmp = dequeue(&q);

		build_index(&index,tmp->val);

		/*adaugare in vectorul de structuri*/

		vector[current_node].red = tmp->val->red;
		vector[current_node].green = tmp->val->green;
		vector[current_node].blue = tmp->val->blue;
		vector[current_node].reserved = tmp->val->reserved;
		vector[current_node].area = tmp->val->area;
		vector[current_node].bottom_left = tmp->val->bottom_left;
		vector[current_node].bottom_right = tmp->val->bottom_right;
		vector[current_node].top_left = tmp->val->top_left;
		vector[current_node].top_right = tmp->val->top_right;
		current_node++;

		/*introducerea in coada a urmatoarelor noduri*/

		if (tmp->val->p4)
			enqueue(&q, tmp->val->p4);
		if (tmp->val->p1)
			enqueue(&q, tmp->val->p1);
		if (tmp->val->p2)
			enqueue(&q, tmp->val->p2);
		if (tmp->val->p3)
			enqueue(&q, tmp->val->p3);

		/*eliberare memorie nod extras*/
		free(tmp);

	}
}

void destroy_tree(QuadtreeNode *root) {

	/*elibereaza memoria ocupata de un arbore*/

	if (!root) return;

	destroy_tree(root->p1);
	destroy_tree(root->p2);
	destroy_tree(root->p3);
	destroy_tree(root->p4);

	free(root);

}

int nleaves(QuadtreeNode *root) {

	/*retureneaza numarul de frunze din arbore*/
	
	int nr = 0;

	if (root) {
		
		if (!root->is_empty) {
			nr++;
		}

		return nr + nleaves(root->p1) + nleaves(root->p2) + nleaves(root->p3) + nleaves(root->p4);
	
	} else {

		return 0;

	}

}

int nodes(QuadtreeNode *root) {

	/*retureneaza numarul de noduri din arbore*/
	
	int nr = 0;

	if (root) {
	
		nr++;
	
		return nr + nodes(root->p1) + nodes(root->p2) + nodes(root->p3) + nodes(root->p4);
	
	} else {
		
		return 0;
	
	}

}


void rebuild_node(QuadtreeNode **root, QuadtreeNode_compressed *buffer, unsigned int position, unsigned int area){

	/*reconstruieste un arbore utilizand forma sa comprimata stocata in vector*/

	init_node(root);
	
	(*root)->red = buffer[position].red;
	(*root)->green = buffer[position].green;
	(*root)->blue = buffer[position].blue;
	(*root)->reserved = buffer[position].reserved;

	(*root)->area = area;

	(*root)->top_left = buffer[position].top_left;
	(*root)->top_right = buffer[position].top_right;
	(*root)->bottom_right = buffer[position].bottom_right;
	(*root)->bottom_left = buffer[position].bottom_left;

	if (!((*root)->top_left == -1 && (*root)->top_right == -1 && (*root)->bottom_right == -1 && (*root)->bottom_left == -1) ){
		
		rebuild_node(&(*root)->p1, buffer, (*root)->top_right, area/4);
		rebuild_node(&(*root)->p2, buffer, (*root)->bottom_right,area/4);
		rebuild_node(&(*root)->p3, buffer, (*root)->bottom_left, area / 4);
		rebuild_node(&(*root)->p4, buffer, (*root)->top_left,area/4);
		
	}

}

void rebuild_node_rotated90(QuadtreeNode **root, QuadtreeNode_compressed *buffer, unsigned int position, unsigned int area){

	/*reconstruieste un arbore utilizand forma sa comprimata stocata in vector*/
	/*va introduce nodurile in arbore in alta ordine decat rebuild_node
	pentru a roti imaginea cu 90 de grade in sens trigonometric*/

	init_node(root);
	
	(*root)->red = buffer[position].red;
	(*root)->green = buffer[position].green;
	(*root)->blue = buffer[position].blue;
	(*root)->reserved = buffer[position].reserved;

	(*root)->area = area;

	(*root)->top_left = buffer[position].top_left;
	(*root)->top_right = buffer[position].top_right;
	(*root)->bottom_right = buffer[position].bottom_right;
	(*root)->bottom_left = buffer[position].bottom_left;

	if (!((*root)->top_left == -1 && (*root)->top_right == -1 && (*root)->bottom_right == -1 && (*root)->bottom_left == -1) ){
		
		rebuild_node_rotated90(&(*root)->p1, buffer, (*root)->bottom_right, area/4);
		rebuild_node_rotated90(&(*root)->p2, buffer, (*root)->bottom_left, area/4);
		rebuild_node_rotated90(&(*root)->p3, buffer, (*root)->top_left, area/4);
		rebuild_node_rotated90(&(*root)->p4, buffer, (*root)->top_right, area/4);
		
	}

}

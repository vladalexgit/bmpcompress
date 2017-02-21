#ifndef _PIXEL_ARRAY_OPERATIONS
	#include "pixel_array_operations.h"
	#define _PIXEL_ARRAY_OPERATIONS
#endif

#ifndef _MATH
	#include <math.h>
	#define _MATH
#endif

#ifndef STRUCT_QUADTREE_NODE
#define STRUCT_QUADTREE_NODE

typedef struct QuadtreeNode {

	unsigned char blue, green, red, reserved;
	
	unsigned int area;
	
	int top_left, top_right;
	int bottom_left, bottom_right;
	
	int is_empty;
	
	struct QuadtreeNode *p1, *p2, *p3, *p4;

}__attribute__((packed)) QuadtreeNode;

#endif

#ifndef STRUCT_QUADTREE_NODE_COMPRESSED
#define STRUCT_QUADTREE_NODE_COMPRESSED

typedef struct {

	unsigned char blue, green, red, reserved;
	
	unsigned int area;
	
	int top_left, top_right;
	int bottom_left, bottom_right;

} __attribute__((packed)) QuadtreeNode_compressed;

#endif

#ifndef STRUCT_LIST_NODE
#define STRUCT_LIST_NODE

typedef struct ListNode {

	QuadtreeNode *val;
	struct ListNode *next;

}__attribute__((packed)) ListNode;

#endif

void enqueue(ListNode **head, QuadtreeNode *val);
ListNode *dequeue(ListNode **head);

void init_node(QuadtreeNode **node);
void insert_into_tree(QuadtreeNode **root, pixel *matrix, int area);
void add_node(QuadtreeNode **node, QuadtreeNode_compressed *buffer);
void bfs(QuadtreeNode *root, QuadtreeNode_compressed *vector);
void destroy_tree(QuadtreeNode *root);

int nleaves(QuadtreeNode *root);
int nodes(QuadtreeNode *root);

void rebuild_node(QuadtreeNode **root, QuadtreeNode_compressed *buffer, unsigned int position, unsigned int area);
void rebuild_node_rotated90(QuadtreeNode **root, QuadtreeNode_compressed *buffer, unsigned int position, unsigned int area);
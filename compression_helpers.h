#ifndef _PIXEL_ARRAY_OPERATIONS
	#include "pixel_array_operations.h"
	#define _PIXEL_ARRAY_OPERATIONS
#endif

#ifndef _QUAD_TREE_OPERATIONS
	#include "quadtree_operations.h"
	#define _QUAD_TREE_OPERATIONS
#endif

#ifndef _BMP_HANDLING
	#include "bmp_handling.h"
	#define _BMP_HANDLING
#endif

#ifndef _STDIO
	#include <stdio.h>
	#define _STDIO
#endif

#ifndef _STDLIB
	#include <stdlib.h>
	#define _STDLIB
#endif

void rebuild_tree(FILE *input_file, QuadtreeNode **root, int nodes, unsigned int area);
pixel* fill_array(QuadtreeNode *node);

int contains_RGB(QuadtreeNode *node, pixel *color);
int is_leaf(QuadtreeNode *node);
QuadtreeNode *find_ancestor(QuadtreeNode *root, pixel *color1, pixel *color2);
void bonus(char *input_file, char *output_file, pixel *color1, pixel *color2);
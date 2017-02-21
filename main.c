#include <stdio.h>
	#define _STDIO
#include <string.h>
	#define _STRING
#include <stdlib.h>
	#define _STDLIB
#include <math.h>
	#define _MATH

#ifndef _BMP_HANDLING
	#include "bmp_handling.h"
	#define _BMP_HANDLING
#endif

#ifndef _PIXEL_ARRAY_OPERATIONS
	#include "pixel_array_operations.h"
	#define _PIXEL_ARRAY_OPERATIONS
#endif

#ifndef _QUADTREE_OPERATIONS
	#include "quadtree_operations.h"
	#define _QUADTREE_OPERATIONS
#endif

#ifndef _COMPRESSION_HELPERS
	#include "compression_helpers.h"
	#define _COMPRESSION_HELPERS
#endif


void decompress(char *input_file, char *output_file);
void compress(char *input_file, char *output_file);
void rotate(char *nr_rotations, char *input_file, char *output_file);


int main(int argc, char **argv) {
	
	if (argc == 4) {
		if (!strcmp(argv[1], "-c")) {
			
			compress(argv[2], argv[3]);
		
		}
		else if (!strcmp(argv[1], "-d")) {

			decompress(argv[2],argv[3]);
		
		}
	}else if(argc == 5){
		if (!strcmp(argv[1], "-r")) {

			rotate(argv[2], argv[3],argv[4]);
		}
	}else if(argc == 10){
		if (!strcmp(argv[1], "-b")) {
			
			pixel color1, color2;

				color1.r = atoi(argv[2]);
				color1.g = atoi(argv[3]);
				color1.b = atoi(argv[4]);
				color2.r = atoi(argv[5]);
				color2.g = atoi(argv[6]);
				color2.b = atoi(argv[7]);

				color1.res = 0;
				color2.res = 0;

				bonus(argv[8], argv[9], &color1, &color2);
		}
	}
	else {
		printf("parametrii sunt gresiti!\n");
	}

	return 0;
}

void compress(char *input_file, char *output_file) {

	/*citire headere*/

	bmp_file_header header;
	bmp_info_header info;

	get_file_header(input_file, &header);
	get_info_header(input_file, &info);

	/*citire "matrice de pixeli"*/
	/*"matricea de pixeli" este reprezentata vectorizat, fiind de fapt un vector de pixeli*/

	int buffer_size = info.height * info.width;

	pixel *buffer = get_pixel_array(input_file, buffer_size);

	/*constructia arborelui*/

	QuadtreeNode *tree = NULL;

	insert_into_tree(&tree, buffer, buffer_size);

	int nr_leaves = nleaves(tree);	
	int nr_nodes = nodes(tree);

	/*transpunere in fisier a arborelui in forma sa comprimata*/

	QuadtreeNode_compressed *vect = malloc(nr_nodes * sizeof(QuadtreeNode_compressed));

	bfs(tree, vect); /*parcurgere + generare vector*/

	/*pregatire fisier*/

	FILE *compressed = fopen(output_file, "wb");

	fwrite(&header, sizeof(bmp_file_header), 1, compressed);
	fwrite(&info, sizeof(bmp_info_header), 1, compressed);
	fwrite(&nr_leaves, sizeof(int), 1, compressed);
	fwrite(&nr_nodes, sizeof(int), 1, compressed);

	/*scriere vecor*/
	int l;
	for (l = 0; l < nr_nodes; l++) {
		fwrite(&vect[l], sizeof(QuadtreeNode_compressed), 1, compressed);
	}

	/*inchidere fisier*/
	fclose(compressed);

	/*eliberarea memoriei*/
	free(vect);
	destroy_tree(tree);

}

void decompress(char *input_file, char *output_file) {

	/*citire headere*/

	bmp_file_header header;
	bmp_info_header info;

	get_file_header(input_file, &header);
	get_info_header(input_file, &info);

	FILE *compressed = fopen(input_file, "rb");

	fseek(compressed, header.offset, SEEK_SET);
	
	int frunze, nodes;

	if(!fread(&frunze, sizeof(unsigned int), 1, compressed)){
		printf("eroare citire nr frunze\n");
		exit(1);
	}

	if(!fread(&nodes, sizeof(unsigned int), 1, compressed)){
		printf("eroare citire nr frunze\n");
		exit(1);
	}

	/*reconstruire arbore*/

	QuadtreeNode *d_tree = NULL;

	unsigned int area = info.height * info.width;

	rebuild_tree(compressed, &d_tree, nodes, area);

	fclose(compressed);

	/*reconstruire "matrice de pixeli" dupa forma arborelui*/

	pixel *pixel_array = fill_array(d_tree);

	/*pregatire fisier output*/
	FILE *decompressed = fopen(output_file,"wb");

	fwrite(&header, sizeof(bmp_file_header), 1, decompressed);
	fwrite(&info, sizeof(bmp_info_header), 1, decompressed);
	
	/*scriere pixeli in fisier*/
	unsigned int l;
	for (l = 0; l < d_tree->area; l++) {
		fwrite(&pixel_array[l], sizeof(pixel), 1, decompressed);
	}
	
	fclose(decompressed);

	/*eliberare memorie*/
	destroy_tree(d_tree);
	free(pixel_array);
}

void rotate(char *nr_rotations, char *input_file, char *output_file){

	int nrot = atoi(nr_rotations);/*nr_rotations[0] - '0';*/ /*conversie char->int*/

	/*citire headere*/
	bmp_file_header header;
	bmp_info_header info;

	get_file_header(input_file, &header);
	get_info_header(input_file, &info);

	/*citire pixeli*/

	unsigned int buffer_size = info.height*info.width;

	pixel *buffer = get_pixel_array(input_file, buffer_size);

	QuadtreeNode *tree = NULL, *r_tree = NULL;
	
	int j;
	for(j=0; j < nrot%4; j++){

		/*generare arbore initial*/
		insert_into_tree(&tree, buffer, buffer_size);

		/*comprimare arbore*/
		QuadtreeNode_compressed *v = malloc(buffer_size * sizeof(QuadtreeNode_compressed));

		bfs(tree, v);

		/*reconstruire arbore rotit*/
		rebuild_node_rotated90(&r_tree, v, 0, buffer_size);

		/*reconstruire matrice de pixeli din arborele rotit*/
		pixel *pixel_array = fill_array(r_tree);	
		
		/*bufferul pentru urmatoarea rotatie va fi format din pixelii matricei rotite*/
		buffer = pixel_array;

		/*eliberare memorie*/
		free(v);
		destroy_tree(r_tree);
		destroy_tree(tree);

	}
	
	/*scriere in fisier imagine rotita*/

	FILE *rotated = fopen(output_file,"wb");

	fwrite(&header, sizeof(bmp_file_header), 1, rotated);
	fwrite(&info, sizeof(bmp_info_header), 1, rotated);

	unsigned int l;
	for (l = 0; l < buffer_size; l++) {
		fwrite(&buffer[l], sizeof(pixel), 1, rotated);
	}

	fclose(rotated);

	/*eliberare memorie*/
	free(buffer);

}

#include "compression_helpers.h"

pixel* fill_array(QuadtreeNode *node) {

	/*functie recursiva*/
	/*reconstruieste "matricea de pixeli" utilizand arborele de reprezentare a unei imagini*/
	/*"matricea de pixeli" este reprezentata vectorizat, fiind de fapt un vector de pixeli*/

	/*in cazul in care functia provoaca o eroare de memorie de tipul access violation,
	 inseamna ca arborele nu a fost construit corect*/

	if (!node->p1 && !node->p2 && !node->p3 && !node->p4) { /*este frunza*/
	
		unsigned int i;
		pixel *buffer = malloc(node->area * sizeof(pixel));
		
		for (i = 0; i < node->area; i++) {
			buffer[i].b = node->blue;
			buffer[i].g = node->green;
			buffer[i].r = node->red;
			buffer[i].res = node->reserved;
		}
		
		return buffer;
	
	} else {
	
		pixel *buffer1 = fill_array(node->p1);
		pixel *buffer2 = fill_array(node->p2);
		pixel *buffer3 = fill_array(node->p3);
		pixel *buffer4 = fill_array(node->p4);
		pixel *main_buffer = malloc(node->area*sizeof(pixel));

		/*reconstructie stanga-jos*/
		unsigned int i, j, offset = 0;
		int k = 0;

		for (j = 0; j < (unsigned int)sqrt(node->area) / 2; j++) {
			for (i = 0; i < (unsigned int)sqrt(node->area) / 2; i++) {
				main_buffer[offset + i] = buffer4[k];
				k++;
			}
			offset += (unsigned int)sqrt(node->area);
		}

		/*reconstructie dreapta-jos*/
		k = 0;
		offset = 0;

		for (j = 0; j < (unsigned int)sqrt(node->area) / 2; j++) {
			for (i = (unsigned int)sqrt(node->area) / 2; i < (unsigned int)sqrt(node->area); i++) {
				main_buffer[offset + i] = buffer3[k];
				k++;
			}
			offset += (unsigned int)sqrt(node->area);
		}

		/*reconstructie dreapta-sus*/
		k = 0;
		offset = node->area / 2;

		for (j = (unsigned int)sqrt(node->area) / 2; j < (unsigned int)sqrt(node->area); j++) {
			for (i = (unsigned int)sqrt(node->area) / 2; i < (unsigned int)sqrt(node->area); i++) {
				main_buffer[offset + i] = buffer2[k];
				k++;
			}
			offset += (unsigned int)sqrt(node->area);
		}
		
		/*reconstructie stanga-sus*/
		k = 0;
		offset = node->area / 2;
		
		for (j = (unsigned int)sqrt(node->area) / 2; j < (unsigned int)sqrt(node->area); j++) {
			for (i = 0; i < (unsigned int)sqrt(node->area)/2; i++) {
				main_buffer[offset + i] = buffer1[k];
				k++;
			}
			offset += (unsigned int)sqrt(node->area);
		}

		free(buffer1);
		free(buffer2);
		free(buffer3);
		free(buffer4);

		return main_buffer;
	}

}

void rebuild_tree(FILE *input_file, QuadtreeNode **root, int nodes, unsigned int area) {

	/*reconstruieste arborele imaginii input_file utilizand forma comprimata a acesteia*/

	QuadtreeNode_compressed *buffer = malloc(nodes * sizeof(QuadtreeNode_compressed));

	/*citire noduri in forma comprimata*/

	unsigned int i = 0;
	while (fread(&buffer[i], sizeof(QuadtreeNode_compressed), 1, input_file)) {
		i++;
	}

	/*reconstructie arbore*/
	rebuild_node(root, buffer, 0, area);

	free(buffer);
}

int contains_RGB(QuadtreeNode *node, pixel *color) {

	/*compara culoarea continuta de un nod din arbore cu cea a unui pixel primit ca parametru*/

	if (node->red == color->r && node->green == color->g && node->blue == color->b) {
	
		return 1;

	} else {
	
		return 0;
	
	}

}

int is_leaf(QuadtreeNode *node) {

	/*verifica daca un nod primit ca parametru este frunza*/

	if (!(node->p1 && node->p2 && node->p3 && node->p4)) {
	
		return 1;
	
	} else {

		return 0;
	
	}

}

QuadtreeNode *find_ancestor(QuadtreeNode *root, pixel *color1, pixel *color2) {

	/*BONUS*/
	/*gaseste cel mai mic stramos ce contine culorile specificate de color1 si color2
	  si returneaza adresa acestuia*/
	/*functie recursiva*/

	if (!root) {
		return NULL;
	}

	if ((contains_RGB(root, color1) || contains_RGB(root, color2)) && is_leaf(root)) {

		return root;

	} else {
		
		QuadtreeNode *q1 = find_ancestor(root->p1, color1, color2);
		QuadtreeNode *q2 = find_ancestor(root->p2, color1, color2);
		QuadtreeNode *q3 = find_ancestor(root->p3, color1, color2);
		QuadtreeNode *q4 = find_ancestor(root->p4, color1, color2);

		int nr = 0;
		
		if (q1)
			nr++;
		if (q2)
			nr++;
		if (q3)
			nr++;
		if (q4)
			nr++;

		if (nr >= 2)
			/*daca nodul curent contine in minim 2 dintre sub-arborii sai
			  culorile respective returnam nodul curent*/
			return root;
			/*altfel daca exista un sub-arbore ce indeplineste conditiile de pana acum
			  il vom returna pe acesta pentru a fi comparat mai departe de catre iteratia
			  precedenta a functiei*/
		else if (q1)
			return q1;
		else if (q2)
			return q2;
		else if (q3)
			return q3;
		else if (q4)
			return q4;
		else
			/*daca nu exista un sub-arbore care sa satisfaca conditiile*/
			return NULL;
	
	}

}

void bonus(char *input_file, char *output_file, pixel *color1, pixel *color2) {

	/*citire headere*/

	bmp_file_header header;
	bmp_info_header info;

	get_file_header(input_file, &header);
	get_info_header(input_file, &info);

	/*citire "matrice de pixeli"*/

	int buffer_size = info.height * info.width;

	pixel *buffer = get_pixel_array(input_file, buffer_size);

	/*constructia arborelui*/

	QuadtreeNode *tree = NULL;

	insert_into_tree(&tree, buffer, buffer_size);

	/*gaseste nodul stramos cautat*/

	QuadtreeNode *querry_result = find_ancestor(tree, color1, color2);


	if (!querry_result){
		printf("eroare nu am putut gasi stramosul\n");
		return;
	}
		
	/*reconstruire "matrice de pixeli" dupa forma sub-arborelui stramos*/

	pixel *pixel_array = fill_array(querry_result);

	/*pregatire fisier output*/
	FILE *decompressed = fopen(output_file, "wb");

	info.width = (int)sqrt(querry_result->area);
	info.height = (int)sqrt(querry_result->area);

	fwrite(&header, sizeof(bmp_file_header), 1, decompressed);
	fwrite(&info, sizeof(bmp_info_header), 1, decompressed);

	/*scriere pixeli in fisier*/
	unsigned int l;
	for (l = 0; l < querry_result->area; l++) {
		fwrite(&pixel_array[l], sizeof(pixel), 1, decompressed);
	}

	fclose(decompressed);

	/*eliberare memorie*/
	destroy_tree(tree);
	free(pixel_array);

}

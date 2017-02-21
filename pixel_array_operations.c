#include "pixel_array_operations.h"

int get_next_pixel(char *file_name, pixel *px) {

	/*primul apel va citi primul pixel din fisier*/
	/*pentru urmatoarele apeluri, 
	  in care se doreste citirea de pixeli din acelasi fisier,
	  file_name trebuie sa fie NULL*/
	/*functioneaza similar cu functia strtok*/

	static FILE *img = NULL;

	/*initializare offset*/
	static long current_offset = TOTAL_HEADER_SIZE;

	if (!img && !file_name) {
		/*daca nu este specificat fisierul*/
		return 0;
	}

	if (file_name) {
		/*se incepe o noua secventa de citire*/
		/*(a fost specificat un nume de fisier)*/

		if (img) {
			/*daca se intrerupe lucrul cu un fisier*/
			fclose(img);
			img = NULL;
		}

		img = fopen(file_name, "rb");
		current_offset = TOTAL_HEADER_SIZE;

		if (!img) {
			printf("Fiserul %s nu a fost gasit!\n", file_name);
			exit(1);
		}

	}

	fseek(img, current_offset, SEEK_SET);

	if (ferror(img) || feof(img)) {
		printf("eroare de citire\n");
		return 0;
	}

	/*citire pixel curent*/
	if (!fread(px, sizeof(pixel), 1, img)){
		fclose(img);
		return 0;
	}

	current_offset += sizeof(pixel);

	return 1; /*(inca se mai pot citi pixeli din fisierul curent)*/

}

pixel *get_pixel_array(char *file_name, int array_size) {

	/*apeleaza succesiv functia get_next_pixel pentru a construi "matricea de pixeli"*/
	/*"matricea de pixeli" este reprezentata vectorizat, fiind de fapt un vector de pixeli*/

	pixel *pixel_array = malloc(array_size * sizeof(pixel));

	get_next_pixel(file_name, &pixel_array[0]);

	int i = 1;

	while (get_next_pixel(NULL, &pixel_array[i])) {

		if (i == array_size) {
			printf("gresit!");
			exit(1);
		}

		i++;

	}

	printf("\n\t inserted %d pixels \n", i);

	return pixel_array; /*memoria ocupata va trebui eliberata ulterior*/ 

}

pixel *get_bot_l(pixel *buffer, int height, int width) {

	/*extrage o "submatrice" formata din pixelii din zona stanga-jos a matricei initiale*/

	int buffer_size = height * width;

	pixel *bot_l = malloc(buffer_size / 4 * sizeof(pixel));

	int i, j, offset = 0, k = 0;

	for (j = 0; j < height / 2; j++) {

		for (i = 0; i < width / 2; i++) {

			bot_l[k] = buffer[offset + i];
			k++;
		
		}
	
		offset += width;
	
	}

	return bot_l;

}

pixel *get_bot_r(pixel *buffer, int height, int width) {

	/*extrage o "submatrice" formata din pixelii din zona dreapta-jos a matricei initiale*/

	int buffer_size = height * width;

	pixel *bot_r = malloc(buffer_size / 4 * sizeof(pixel));

	int i,j, offset = 0, k = 0;

	for (j = 0; j < height / 2; j++) {
		
		for (i = width / 2; i < width; i++) {
		
			bot_r[k] = buffer[offset + i];
			k++;
		
		}
		
		offset += width;
	
	}

	return bot_r;

}

pixel *get_top_l(pixel *buffer, int height, int width) {

	/*extrage o "submatrice" formata din pixelii din zona stanga-sus a matricei initiale*/

	int buffer_size = height * width;

	pixel *top_l = malloc(buffer_size / 4 * sizeof(pixel));

	int i, j, offset = buffer_size / 2, k = 0;

	for (j = height / 2; j < height; j++) {
		
		for (i = 0; i < width / 2; i++) {
			
			top_l[k] = buffer[offset + i];
			k++;
		
		}
		
		offset += width;
	
	}

	return top_l;

}

pixel *get_top_r(pixel *buffer, int height, int width) {

	/*extrage o "submatrice" formata din pixelii din zona dreapta-sus a matricei initiale*/

	int buffer_size = height * width;

	pixel *top_r = malloc(buffer_size / 4 * sizeof(pixel));

	int i, j, offset = buffer_size / 2, k = 0;

	for (j = height / 2; j < height; j++) {
	
		for (i = width / 2; i < width; i++) {
	
			top_r[k] = buffer[offset + i];
			k++;
	
		}
	
		offset += width;
	}

	return top_r;

}

int has_same_color(pixel *array, int size) {

	/*verifica daca toti pixelii din matrice au aceeasi valoare*/

	int i;

	pixel ref = array[0];

	for (i = 0; i < size; i++) {
	
		if (array[i].b != ref.b || array[i].g != ref.g || array[i].r != ref.r)
			return 0;
	
	}

	return 1;

}

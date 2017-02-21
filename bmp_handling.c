#include "bmp_handling.h"

#define FILE_HEADER_SIZE 14
#define TOTAL_HEADER_SIZE 54


void get_file_header(char *file_name, bmp_file_header *header) {

	/* citeste elementele structurii file_header din fisier*/

	FILE *picture = fopen(file_name, "rb");

	if (!picture) {
		printf("Fiserul %s nu a fost gasit!\n", file_name);
		exit(1);
	}

	if(!fread(header,sizeof(bmp_file_header),1,picture)){
		printf("eroare citire header!");
		exit(1);
	}

	fclose(picture);

}

void print_file_header(bmp_file_header *header) {

	printf("signature: %c%c\n", header->signature1, header->signature2);
	printf("reserved: %d\n", header->reserved);
	printf("offset: %d\n\n", header->offset);

}

void get_info_header(char *file_name, bmp_info_header *info) {

	/* citeste elementele structurii info_header din fisier*/

	FILE *picture = fopen(file_name, "rb");

	if (!picture) {
		printf("Fiserul %s nu a fost gasit!\n", file_name);
		exit(1);
	}

	fseek(picture, FILE_HEADER_SIZE, SEEK_SET);

	if(!fread(info,sizeof(bmp_info_header),1,picture)){
		printf("eroare citire header!\n");
		exit(1);
	}

	fclose(picture);
}

void print_info_header(bmp_info_header *info) {

	printf("size: %d\n", info->size);
	printf("width: %d\n", info->width);
	printf("height: %d\n", info->height);

	printf("planes: %d\n", info->planes);
	printf("bit_count: %d\n", info->bit_count);
	printf("compression: %d\n", info->compression);
	printf("image_size: %d\n", info->image_size);
	printf("x_pixels_per_meter: %d\n", info->x_pixels_per_meter);
	printf("y_pixels_per_meter: %d\n", info->y_pixels_per_meter);

	printf("colors_used: %d\n", info->colors_used);
	printf("colors_important: %d\n\n", info->colors_important);

}

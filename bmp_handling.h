#ifndef _STDIO
	#include <stdio.h>
	#define _STDIO
#endif

#ifndef _STDLIB
	#include <stdlib.h>
	#define _STDLIB
#endif

#ifndef STRUCT_BMP_FILE_HEADER
#define STRUCT_BMP_FILE_HEADER

typedef struct {

	char signature1;
	char signature2;
	int file_size;
	int reserved;
	int offset;

} __attribute__((packed)) bmp_file_header;

#endif

#ifndef STRUCT_BMP_INFO_HEADER
#define STRUCT_BMP_INFO_HEADER

typedef struct {

	int size;
	int width;
	int height;

	short planes;
	short bit_count;

	int compression;
	int image_size;
	int x_pixels_per_meter;
	int y_pixels_per_meter;
	int colors_used;
	int colors_important;

} __attribute__((packed)) bmp_info_header;

#endif

void get_file_header(char *file_name, bmp_file_header *header);
void print_file_header(bmp_file_header *header);

void get_info_header(char *file_name, bmp_info_header *info);
void print_info_header(bmp_info_header *info);

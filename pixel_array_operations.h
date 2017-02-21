#ifndef TOTAL_HEADER_SIZE
	#define TOTAL_HEADER_SIZE 54
#endif

#ifndef _STDIO
	#include <stdio.h>
	#define _STDIO
#endif

#ifndef _STDLIB
	#include <stdlib.h>
	#define _STDLIB
#endif

#ifndef STRUCT_PIXEL
#define STRUCT_PIXEL

typedef struct {

	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char res;

} __attribute__((packed)) pixel;

#endif

int get_next_pixel(char *file_name, pixel *px);

pixel *get_pixel_array(char *file_name, int array_size);

pixel *get_bot_l(pixel *buffer, int height, int width);
pixel *get_bot_r(pixel *buffer, int height, int width);
pixel *get_top_l(pixel *buffer, int height, int width);
pixel *get_top_r(pixel *buffer, int height, int width);

int has_same_color(pixel *array, int size);

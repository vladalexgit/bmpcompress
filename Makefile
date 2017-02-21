CC=gcc
CFLAGS= -Wall -O2
DEPENDINTE=pixel_array_operations.o bmp_handling.o quadtree_operations.o compression_helpers.o main.o

all: build

build: quadtree

quadtree: $(DEPENDINTE)
	$(CC) $(CFLAGS) $(DEPENDINTE) -o ./quadtree -lm

bmp_handling.o: bmp_handling.c
	$(CC) -c $(CFLAGS) bmp_handling.c -o ./bmp_handling.o

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o ./main.o

pixel_array_operations.o: pixel_array_operations.c
	$(CC) -c $(CFLAGS) pixel_array_operations.c -o pixel_array_operations.o

quadtree_operations.o: quadtree_operations.c
	$(CC) -c $(CFLAGS) quadtree_operations.c -o quadtree_operations.o

compression_helpers.o: compression_helpers.c
	$(CC) -c $(CFLAGS) compression_helpers.c -o compression_helpers.o

run: quadtree
	./quadtree

clean:
	-rm -rf $(DEPENDINTE) ./quadtree

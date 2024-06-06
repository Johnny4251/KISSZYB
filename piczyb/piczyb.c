#include <stdio.h>
#include <kisszyb.h>
#include <stdlib.h>

#define WIDTH 250
#define HEIGHT 250

int main(int argc, char **argv) {
	
	if(argc < 2) {
		printf("USAGE %s <img.zyb>\n", argv[0]);
		return -1;
	}
	char *img = argv[1];
	printf("image: %s\n", img);

	FILE *file = fopen(img, "r");
	if(!file) {
		printf("could not open image\n");
		return -1;
	}
	
	int img_array[WIDTH][HEIGHT];
	for(int i=0; i<WIDTH; i++){
		for(int j=0; j<HEIGHT; j++){
			char buff[100];
			fgets(buff, sizeof(buff), file);
			int num = atoi(buff);
			img_array[i][j] = num;
		}
	}

	fclose(file);
	

	return 0;
}

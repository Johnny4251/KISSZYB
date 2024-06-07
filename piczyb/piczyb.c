#include <stdio.h>
#include <kisszyb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
	
	// height, width or width, height??
	int img_array[WIDTH][HEIGHT];
	for(int i=0; i<HEIGHT; i++) {
		for(int j=0; j<WIDTH; j++) {
			char buff[100];
			fgets(buff, sizeof(buff), file);
			int num = atoi(buff);
			img_array[i][j] = num;
		}
	}
	
	for(int i=0; i < HEIGHT; i++) {
		
		AX25Frame frame;
	        strncpy(frame.dest_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
        	strncpy(frame.source_callsign, "CQTEST-1", CALLSIGN_MAX_LEN);

		char buffer[WIDTH] = {0};
		for(int j=0; j<WIDTH; j++) {
			buffer[j] = (char)img_array[i][j];
			//printf("num: %d:", buffer[j]);
			//printf("%c\n", buffer[j]);
		}
		
		strcpy(frame.data, buffer);
		send_to_direwolf(&frame, "127.0.0.1", 8001);
		sleep(1);
		printf("row %d complete\n", i);
	}




	fclose(file);
	return 0;
}

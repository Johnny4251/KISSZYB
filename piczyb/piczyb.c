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

	FILE *file = fopen(img, "r");
	if(!file) {
		printf("could not open image\n");
		return -1;
	}
	
	printf("\n-----PICZYB-----\n\n");
	printf("IMAGE: \t\t%s\n",img);

	// build buffer
	int img_array[WIDTH][HEIGHT];
	for(int i=0; i<WIDTH; i++) {
		for(int j=0; j<HEIGHT; j++) {
			char buff[100];
			fgets(buff, sizeof(buff), file);
			int num = atoi(buff);
			img_array[i][j] = num;
		}
	}
	
	// send two empty frames
	for(int i=0; i < 2; i++) {
		AX25Frame frame;
		strncpy(frame.dest_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
                strncpy(frame.source_callsign, "CQTEST-1", CALLSIGN_MAX_LEN);
		strcpy(frame.data, "\\#");
                send_to_direwolf(&frame, "127.0.0.1", 8001);
	}

	// TX Buffer
	for(int i=0; i < HEIGHT; i++) {
		
		AX25Frame frame;
	        strncpy(frame.dest_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
        	strncpy(frame.source_callsign, "CQTEST-1", CALLSIGN_MAX_LEN);
		char buffer[WIDTH] = {0};
		for(int j=0; j<WIDTH; j++) {
			buffer[j] = (char)img_array[i][j];
		}

		strcpy(frame.data, buffer);
		send_to_direwolf(&frame, "127.0.0.1", 8001);

		// give time for TX
		sleep(2);
		
		float percent = (i/(float)HEIGHT) * 100;
		printf("\rTX_STATUS:\t%.2f%% COMPLETE  ", percent);
		fflush(stdout);
	}
	printf("\rTX_STATUS:\t100.00%% COMPLETE  ");
	fflush(stdout);
	
	printf("\nsending END signal...\n");
	AX25Frame endFrame;
        strncpy(endFrame.dest_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
        strncpy(endFrame.source_callsign, "CQTEST-1", CALLSIGN_MAX_LEN);
        strcpy(endFrame.data, "END");
	for(int i=0; i < 10; i++) {
		send_to_direwolf(&endFrame,"127.0.0.1", 8001);
		sleep(1);
	}

	printf("\n----------------\n");
	
	fclose(file);
	return 0;
}

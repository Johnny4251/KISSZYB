#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kisszyb.h>

#define HEIGHT 250
#define WIDTH 250

int main(int argc, char **argv) {

	if(argc < 2) {
		printf("USAGE: %s <image_path>\n", argv[0]);
		return -1;
	}

	char *filepath = (char*) malloc(sizeof(argv[1]));
	strcpy(filepath, argv[1]);

	FILE *file = fopen(filepath, "r");
	if(file) {
		printf("File already exists.. overwrite? ");
		char response;
	        scanf(" %c", &response);

		if (response == 'y' || response == 'Y') {
                	printf("overwriting %s\n", filepath);
		} else {
			fclose(file);
			printf("exiting...\n");
			return -1;
		}
		fclose(file);
        }
	
	file = fopen(filepath, "w");

	fclose(file);

	while(1) {
		char row[250];
		recv_from_direwolf(row, 512, "127.0.0.1", 8001);
		if(strlen(row) > 2) {
	            printf("row: %s\n\n", row+2);
		    for(int i=0; i<HEIGHT; i++) {
			    printf("%c ", row[i]);
		    }
		    printf("\n");
		    //break;
        	}
	}

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kisszyb.h>

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
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <kisszyb.h>

FILE *file = NULL;

void signal_handler(int signal_number) {
	if (file != NULL) {
		fclose(file);
		file = NULL;
	}
	exit(signal_number);
}


int main(int argc, char **argv) {
	
	signal(SIGINT, signal_handler);

	// parse flags
	int opt;
	int outputFlag = 0;
	char *filepath = NULL;

	while((opt = getopt(argc, argv, "o:")) != -1) {
		switch(opt) {
			case 'o':
				outputFlag = 1;
				filepath = optarg;
				break;
			default:
				printf("Usage: %s [-o output_file]\n", argv[0]);
				return -1;
		}
	}

	file = fopen(filepath, "w");
	if(outputFlag == 1 & !file) {
		printf("Can not write to: %s\n", filepath);
		return -1;
	}
	
	char buffer[512];
	while(1) {
		recv_from_direwolf(buffer, 512, "127.0.0.1", 8001);
		if(strlen(buffer) > 2) {
			printf("%s\n", buffer+2);
			if(outputFlag == 1) {
				fprintf(file, "%s\n", buffer+2);
				fflush(file);
			}	
			
		}
	}

	if (file) {
		fclose(file);
	}

	return 0;
}

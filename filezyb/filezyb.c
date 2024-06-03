#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kisszyb.h>

#define BUFFER_SIZE 255
#define DEST_CALLSIGN "CQTEST-0"
#define SOURCE_CALLSIGN "CQTEST-1"


int main(int argc, char **argv) {

	if(argc < 2) {
		printf("USAGE: %s <filepath>\n", argv[0]);
		return -1;
	}

	char *filepath = (char*) malloc(strlen(argv[1])+1);
	strcpy(filepath, argv[1]);

	FILE* file = fopen(filepath, "r");	
	if(file == NULL) {
		perror("Could not open file");
		return -1;
	}
	free(filepath);
	
	// send nothing x2
	for(int i=0; i<2; i++) {
		AX25Frame frame;
        	strncpy(frame.dest_callsign, DEST_CALLSIGN, CALLSIGN_MAX_LEN);
	        strncpy(frame.source_callsign, SOURCE_CALLSIGN, CALLSIGN_MAX_LEN);
        	strcpy(frame.data, "\\#");
	        send_to_direwolf(&frame, "127.0.0.1", 8001);
	}
	

	char *buffer = (char*) malloc(BUFFER_SIZE);;
	while(fgets(buffer, BUFFER_SIZE, file)) {
		// strip newline
		buffer[strcspn(buffer, "\n")] = '\0';
		
		size_t curr_len = strlen(buffer);
		if (curr_len + 3 > BUFFER_SIZE) {

			char *new_buffer = realloc(buffer, curr_len + 3);		
			if (!new_buffer) {
     				free(buffer);
        	        	perror("Failed to reallocate memory");
	                	exit(1);
    			}
			buffer = new_buffer;
		}
    	   	strcat(buffer, "\\#");

		printf("%s\n", buffer);
		
		AX25Frame frame;
		strncpy(frame.dest_callsign, DEST_CALLSIGN, CALLSIGN_MAX_LEN);
		strncpy(frame.source_callsign, SOURCE_CALLSIGN, CALLSIGN_MAX_LEN);
		
		strcpy(frame.data, buffer);
		send_to_direwolf(&frame, "127.0.0.1", 8001);
	
	}

	free(buffer);
	fclose(file);
	return 0;
}

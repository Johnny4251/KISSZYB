#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <kisszyb.h>

void send_data(char *data) {
	printf("sent: %s\n", data);

	AX25Frame dataFrame;
        strncpy(dataFrame.dest_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
        strncpy(dataFrame.source_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
        strcpy(dataFrame.data, data);

        send_to_direwolf(&dataFrame, "127.0.0.1", 8001);
}

void send_header(int clength) {
	char header[100];
       sprintf(header,
    		"HTTP/1.1 200 OK\r\n"
	        "Content-Type: text/html\r\n"
	        "Connection: close\r\n"
    		"Content-Length:%d\r\n"
	        "\r\n\\#", clength-11);
	send_data(header);
}

int get_newlines(char *file_path) {
        FILE *file = fopen(file_path, "r");
        char dummy[512];
        int linecount = 0;
        while(fgets(dummy, sizeof(dummy), file)) {
                linecount++;
        }
        fclose(file);
        return linecount;
}

int get_file_size(char *file_path) {
	struct stat st;
	if (stat(file_path, &st) == 0)
	        return (int)st.st_size;
	printf("can not get file size\n");
	return -1;
}

void send_page(char *file_path) {
	printf("sending: %s\n", file_path);
	int clength = get_file_size(file_path);
	int linecount = get_newlines(file_path);
	clength -= linecount;
	send_header(clength);
	FILE *file = fopen(file_path, "r");

	char *buffer = (char*) malloc(256);
	while(fgets(buffer, 256, file)) {
		buffer[strcspn(buffer, "\n")] = '\0';
		size_t curr_len = strlen(buffer);
		 if (curr_len + 3 > 256) {

                        char *new_buffer = realloc(buffer, curr_len + 3);
                        if (!new_buffer) {
                                free(buffer);
                                perror("Failed to reallocate memory");
                                exit(1);
                        }
                        buffer = new_buffer;
                }
		strcat(buffer, "\\#");
		send_data(buffer);
	}
	send_data("--ENDPAGE--\\#");
	

	fclose(file);
	return;
}

void send_404() {
	char *msg = "404 Not Found\\#";
	send_data("\\#");
	send_data(msg);
	return;
}

int main(int argc, char **argv) {
	
	if(argc < 2) {
		printf("USAGE: %s <example.html>\n", argv[0]);
		return 0;
	}

	char *file_path = (char*) malloc(strlen(argv[1]));
	file_path = argv[1];
	
	FILE *file = fopen(file_path, "r");
	if(!file) {
		printf("Error: Opening: %s\n", file_path);
		return -1;
	}
	fclose(file);

	while(1) {
		char data[512];
		recv_from_direwolf(data, sizeof(data), "127.0.0.1", 8001);
		if(strlen(data) > 3) {
			printf("\nrecv: %s\n", data+2);
			printf("waiting to start TX...\n");
        		sleep(5);
			if(strncmp(data+2, "GET", strlen("GET")) == 0) {
				send_data("200\\#");
				sleep(7);
				send_page(file_path);
			} else {
				send_404();		
			}
		}
	}

	free(file_path);
	fclose(file);
	return 0;
}

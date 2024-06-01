#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <kisszyb.h>

pthread_t listen_thread;
pthread_t send_thread;

void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void signal_handler(int signal_number) {
	exit(signal_number);
}

void* listen_job(void *arg) {
	char data[512];
	while (1) {
		recv_from_direwolf(data, sizeof(data), "127.0.0.1", 8001);
		if(strlen(data) > 3) {
			printf("\nRecv: %s\n", data+2);
		}
	}
	return NULL;
}

void* send_job(void* arg) {
	while(1) {
		char data[512];
		printf("\nWhat would you like to send: ");
		//flush_stdin();
		fgets(data, sizeof(data), stdin);
		data[strcspn(data, "\n")] - '\0';
		strcat(data, "\\#");

		AX25Frame myFrame;
        	strncpy(myFrame.dest_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
	        strncpy(myFrame.source_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);

        	strcpy(myFrame.data, "\\#");
	        send_to_direwolf(&myFrame, "127.0.0.1", 8001);
        	strcpy(myFrame.data, data);
	        send_to_direwolf(&myFrame, "127.0.0.1", 8001);
	}
}

int main(int argc, char **argv) {
	signal(SIGINT, signal_handler);
	
	pthread_create(&listen_thread, NULL, listen_job, NULL);
	pthread_create(&send_thread, NULL, send_job, NULL);
	pthread_join(listen_thread, NULL);
	pthread_join(send_thread, NULL);
	
	return 0;
}

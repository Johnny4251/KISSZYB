#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <kisszyb.h>
#include <semaphore.h>

int *accepted;
sem_t sem;

void send_wolf(char *data) {
    printf("sent: %s\n", data);

    AX25Frame dataFrame;
    strncpy(dataFrame.dest_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
    strncpy(dataFrame.source_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
    strcpy(dataFrame.data, data);

    send_to_direwolf(&dataFrame, "127.0.0.1", 8001);
}

void listen_for_accept() {
    char buffer[100];
    printf("waiting...\n");
    while(1) {
        recv_from_direwolf(buffer, 512, "127.0.0.1", 8001);
        printf("server sent: %s\n", buffer+2);
        if(strncmp(buffer+2, "200", strlen("200")) == 0) {
            sem_wait(&sem);
            *accepted = 1;
            sem_post(&sem);
            printf("accepted!\n");
            break;
        }
    }
    exit(0);
}

int main(int argc, char **argv){
    if(argc < 2) {
        printf("USAGE: %s <port>\n", argv[0]);
        return -1;
    }
    int port = atoi(argv[1]);

    accepted = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *accepted = 0;

    sem_init(&sem, 0, 1);

    int sockfd, clientfd;
    socklen_t clientlen;
    struct sockaddr_in server_addr, client_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR: opening socket\n");
        return -1;
    }

    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("ERROR: binding socket\n");
        return -1;
    }

    listen(sockfd, 5);
    clientlen = sizeof(client_addr);

    while(1) {
	sleep(3);
	char buffer[255] = {0};
        clientfd = accept(sockfd, (struct sockaddr *) &client_addr, &clientlen);
        read(clientfd, buffer, sizeof(buffer));

        if(strncmp(buffer, "GET", strlen("GET")) != 0) {
            continue;
        }

        if(fork() == 0) {
            listen_for_accept();
        }

        send_wolf("GET\\#");
        sleep(10);

        for(int i = 0; i <= 5; i++){
            sem_wait(&sem);
            if(*accepted) {
                sem_post(&sem);
                break;
            }
            sem_post(&sem);
            printf("trying again for response...\n");
            send_wolf("GET\\#");
            sleep(10);
        }

        sem_wait(&sem);
        if(!*accepted) {
	    printf("timed out!\n");
            sem_post(&sem);
            break;
        }
        sem_post(&sem);

	printf("PAGE CONTENT\n");
        while(1) {
            recv_from_direwolf(buffer, sizeof(buffer), "127.0.0.1", 8001);
            if(strncmp(buffer+2, "--ENDPAGE--", strlen("--ENDPAGE--")) == 0) {
                break;
            }
            if(strlen(buffer) > 2) {
                printf("%s\n", buffer+2);
                write(clientfd, buffer+2, strlen(buffer)-2);
            }
        }
    }

    printf("exiting...\n");
    close(sockfd);
    close(clientfd);
    sem_destroy(&sem);
    return 0;
}


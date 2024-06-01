/*
 * Author:	John Pertell
 * Date: 	04.29.24
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CALLSIGN_MAX_LEN 7
#define DATA_MAX_LEN 256
#define AX25_CTRL_UI_FRAME 0x03
#define AX25_PID_NO_LAYER3 0xF0

#define KISS_FEND 0xC0
#define KISS_FESC 0xDB
#define KISS_TFEND 0xDC
#define KISS_TFESC 0xDD

typedef struct {
    char dest_callsign[CALLSIGN_MAX_LEN];
    char source_callsign[CALLSIGN_MAX_LEN];
    char data[DATA_MAX_LEN];
} AX25Frame;

void encode_address(const char *callsign, int ssid, char final, unsigned char *encoded_addr) {
    char call[7];
    snprintf(call, 7, "%-6s", callsign);
    for (int i = 0; i < 6; i++) {
        encoded_addr[i] = (call[i] << 1);
    }
    encoded_addr[6] = ((ssid & 0x0F) << 1) | 0x60 | (final ? 0x01 : 0x00);
}

void build_ax25_ui_frame(const AX25Frame *frame, unsigned char *ax25_packet, size_t *packet_len) {
    unsigned char dest_addr[7], src_addr[7];
    encode_address(frame->dest_callsign, 0, 0, dest_addr);
    encode_address(frame->source_callsign, 0, 1, src_addr);

    unsigned char ctrl_field = AX25_CTRL_UI_FRAME;
    unsigned char pid_field = AX25_PID_NO_LAYER3;
    unsigned char info_field[DATA_MAX_LEN];
    int info_len = snprintf((char *)info_field, DATA_MAX_LEN, "%s", frame->data);

    memcpy(ax25_packet, dest_addr, 7);
    memcpy(ax25_packet + 7, src_addr, 7);
    ax25_packet[14] = ctrl_field;
    ax25_packet[15] = pid_field;
    memcpy(ax25_packet + 16, info_field, info_len);

    *packet_len = 16 + info_len;
}

void kiss_frame_encode(const unsigned char *ax25_packet, size_t packet_len, unsigned char *kiss_packet, size_t *kiss_len) {
    size_t i = 0;
    kiss_packet[i++] = KISS_FEND;
    kiss_packet[i++] = 0;

    for (size_t j = 0; j < packet_len; j++) {
        unsigned char byte = ax25_packet[j];
        switch (byte) {
            case KISS_FEND:
                kiss_packet[i++] = KISS_FESC;
                kiss_packet[i++] = KISS_TFEND;
                break;
            case KISS_FESC:
                kiss_packet[i++] = KISS_FESC;
                kiss_packet[i++] = KISS_TFESC;
                break;
            default:
                kiss_packet[i++] = byte;
        }
    }
    kiss_packet[i++] = KISS_FEND;
    *kiss_len = i;
}

void send_to_direwolf(const AX25Frame *frame, const char *ip, int port) {
    int sock;
    struct sockaddr_in server;

    unsigned char ax25_packet[1024], kiss_packet[2048];
    size_t ax25_len = 0, kiss_len = 0;

    build_ax25_ui_frame(frame, ax25_packet, &ax25_len);
    kiss_frame_encode(ax25_packet, ax25_len, kiss_packet, &kiss_len);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Could not create socket");
        return;
    }

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connect failed");
        return;
    }

    if (send(sock, kiss_packet, kiss_len, 0) < 0) {
        perror("Send failed");
        return;
    }

    close(sock);
    sleep(1); //prevents dumb mistakes.. trust me
}

void decode_data(char *buff, int buff_size) {
	int data_length = buff_size - 17;

    	memmove(buff, buff + 16, data_length);
    	char *end_delimiter = strrchr(buff, '#');

    	if (end_delimiter && *(end_delimiter - 1) == '\\') {
        	*(end_delimiter - 1) = '\0';
   	}
}


void recv_from_direwolf(char *buff, int buff_size, const char *ip, int port) {
    int sock;
    struct sockaddr_in server_address;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &server_address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return;
    }

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection Failed");
        return;
    }

    read(sock, buff, buff_size);
    close(sock);

    decode_data(buff, buff_size);

}


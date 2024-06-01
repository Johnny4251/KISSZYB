#ifndef KISSZYB_H
#define KISSZYB_H

#include <stddef.h>  // for size_t

// Constants
#define CALLSIGN_MAX_LEN 7
#define DATA_MAX_LEN 256

// Structure for an AX.25 frame
typedef struct {
    char dest_callsign[CALLSIGN_MAX_LEN];
    char source_callsign[CALLSIGN_MAX_LEN];
    char data[DATA_MAX_LEN];
} AX25Frame;

// Function to initialize an AX.25 frame with provided data
void init_ax25_frame(AX25Frame *frame, const char *dest, const char *source, const char *info);

// Function to send an AX.25 frame to Direwolf via network
void send_to_direwolf(const AX25Frame *frame, const char *ip, int port);

// Function to recv an AX.25 frame from Direwolf via network
void recv_from_direwolf(char *buff, int buff_size, const char *ip, int port);

#endif // KISSZYB_H


#include <kisszyb.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char**argv) {

        if(argc < 2) {
                printf("USAGE: %s <MESSAGE>\n", argv[0]);
                return 0;
        }

        AX25Frame myFrame;

        strncpy(myFrame.dest_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
        strncpy(myFrame.source_callsign, "CQTEST-1", CALLSIGN_MAX_LEN);

        strcpy(myFrame.data, argv[1]);
        send_to_direwolf(&myFrame, "127.0.0.1", 8001);

        return 0;
}

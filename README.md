
# KISSZYB

KISSZYB is a C library designed to facilitate communication using the KISS (Keep It Simple Stupid) protocol, commonly used in amateur radio and AX.25 packet radio networks. This library provides a straightforward interface for encoding and decoding KISS frames. KISSZYB is designed specifically to work alongside a Direwolf TNC. The Direwolf project can be found at [Direwolf GitHub Repository](https://github.com/wb2osz/direwolf).

## Features

- Encode and decode KISS frames
- Support for AX.25 protocol
- Examples for sending and receiving frames
- Easy integration with existing projects

## Installation

To build and install the library, run the following commands:

```sh
make
sudo make install
```

## Usage

Include the `kisszyb.h` header in your project and link against the KISSZYB library.

### Sending a Frame

```c
#include <string.h>
#include "kisszyb.h"

int main() {
    // Strings using kisszyb are delimitted by \# escape sequence.
    char buffer[256] = "Hello, World!\\#";
    AX25Frame myFrame;

    strncpy(myFrame.dest_callsign, "CQTEST-0", CALLSIGN_MAX_LEN);
    strncpy(myFrame.source_callsign, "CQTEST-1", CALLSIGN_MAX_LEN);

    strcpy(myFrame.data, buffer);
    send_to_direwolf(&myFrame, "127.0.0.1", 8001);
    return 0;
}
```

### Receiving a Frame

```c
#include <string.h>
#include "kisszyb.h"

int main(int argc, char **argv) {
    char buffer[256];
    while(1) {
        recv_from_direwolf(buffer, 512, "127.0.0.1", 8001);
        if(strlen(buffer) > 2) { 
            printf("%s\n", buffer+2);
        }
    }

    return 0;
}
```

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

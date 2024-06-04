
# filezyb

filezyb is a C program designed for transferring files over AX.25 packet radio networks using the KISS protocol. It utilizes the KISSZYB library for encoding and decoding KISS frames, and works in conjunction with the Direwolf TNC.

## Features

- Transfer files over AX.25 packet radio networks
- Utilizes the KISSZYB library for encoding and decoding KISS frames
- Easy integration with existing AX.25 packet radio setups

## Requirements

- KISSZYB library
- Direwolf TNC
- GCC (for compilation)

## Installation

1. Ensure that the KISSZYB library is installed on your system. Follow the instructions in the [KISSZYB repository](https://github.com/Johnny4251/KISSZYB) to build and install the library.
2. Clone this repository:
    ```sh
    git clone https://github.com/Johnny4251/filezyb.git
    cd filezyb
    ```
3. Build the `filezyb` executable using the provided `Makefile`:
    ```sh
    make
    ```

## Usage

To transfer a file using `filezyb`, run the following command:

```sh
./filezyb <filepath>
```

Replace `<filepath>` with the path to the file you want to transfer.

### Example

```sh
./filezyb example.txt
```

## Code Overview

The `filezyb` program reads the contents of a file and sends it over an AX.25 packet radio network using the KISS protocol. The program sends two empty frames at the beginning of the transfer, then sends each line of the file as a separate frame. The empty frames serve as a quick buffer to ensure both radios are waken up before TX/RX. Each frame is delimited by the `\#` sequence.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.


# webzyb

webzyb is a C project designed to facilitate web communication over AX.25 packet radio networks using the KISS protocol. It includes two components: a client and a host, which work together to send and receive web page data. The project utilizes the KISSZYB library for encoding and decoding KISS frames and works in conjunction with the Direwolf TNC.

## Features

- Web communication over AX.25 packet radio networks
- Utilizes the KISSZYB library for encoding and decoding KISS frames
- Easy integration with existing AX.25 packet radio setups
- Client and Host components for sending and receiving web page data

## Requirements

- KISSZYB library
- Direwolf TNC
- GCC (for compilation)

## Installation

1. Ensure that the KISSZYB library is installed on your system. Follow the instructions in the [KISSZYB repository](https://github.com/Johnny4251/KISSZYB) to build and install the library.
2. Clone this repository:
    ```sh
    git clone https://github.com/Johnny4251/webzyb.git
    cd webzyb
    ```
3. Build the `webzyb` executables using the provided `Makefile`:
    ```sh
    make
    ```

## Usage

### Starting the Host

The host serves a static HTML page. To start the host, run:

```sh
./webzyb-host <example.html>
```

Replace `<example.html>` with the path to your HTML file.

### Starting the Client

The client works by requesting a page from the host and then hosting it over a TCP socket on localhost. This request can be sent by the web browser on a localhost URI. 

```sh
./webzyb-client <port>
```

Replace `<port>` with the port number to host the client on (e.g., 8080).

### Example

Host:
```sh
./webzyb-host example.html
```

Client:
```sh
./webzyb-client 8080
```

## How It Works

- The **Host**: The host program serves a static HTML page over the AX.25 packet radio network. It listens for GET requests from the client and responds with the content of the HTML file.
- The **Client**: The client program sends a GET request to the host via the AX.25 network. This can be done by entering the localhost URI into a browser. Upon receiving the response, it hosts the received page over the TCP socket on localhost. This allows a web browser to connect to the client and view the served page. Essentially, the client acts as a middleman in between the web browser and data received via Direwolf TNC.


## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.


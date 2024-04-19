#pragma once

#include <netinet/in.h>  // sockaddr_in
#include <stdio.h>       // FILE

enum { BUFFER_SIZE = 1024 };

/**
 * Attempt to connect to a server on a given socket.
 *
 * Given an unconnected socket and a server address/port, attempt to connect to
 * a server. If successful, the socket descriptor client_socket can be used to
 * communicate with the server. If not, print an error message and exit without
 * returning from the function.
 *
 * @param client_socket The client's socket descriptor used for the connection.
 * @param server_addr The IPv4 socket address and port of the server.
 */
void try_connect(int client_socket, struct sockaddr_in server_addr);

void create_listener(int client_socketFD);

void listen_and_print(int client_socketFD);

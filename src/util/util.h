#pragma once

#define SERVER_IP INADDR_ANY

enum {
  RECV_BUFFER_SIZE = 1024,
  SERVER_PORT = 12345,
};

#include <netinet/in.h>  // port, struct sockaddr_in, in_addr_t, in_port_t
#include <stdio.h>
#include <stdnoreturn.h>  // noreturn

/**
 * Print an error message and exit with a failure status code.
 *
 * NOTE: This function was taken from Assignment 8
 *
 * Upon an error, print an error message with a desired prefix. The prefix
 * error_msg should describe the context in which the error occurred, followed
 * by a more specific message corresponding to errno set by whatever function or
 * system call that encountered the error. This function exits the program and
 * thus does not return.
 *
 * @param error_msg The error message to print.
 */
noreturn void error_and_exit(const char* error_msg);

/**
 * Attempt to open an IPv4 TCP socket.
 *
 * NOTE: This function was taken from Assignment 8
 *
 * Try to create a TCP socket on IPv4, and return the socket descriptor if
 * successful. If creating the socket is not successful, print an error message
 * and exit the program without returning from the function.
 *
 * @return A socket descriptor corresponding to the new socket.
 */
int open_tcp_socket(void);

/**
 * Attempt to close an open socket.
 *
 * NOTE: This function was taken from Assignment 8
 *
 * Try to close an existing socket given its descriptor. If closing the socket
 * is not successful, print an error message and exit the program. In the case
 * of an error, this function does not return.
 *
 * @param socket_descriptor The descriptor for the socket to close.
 */
void close_tcp_socket(int socket_descriptor);

/**
 * Return the sockaddr_in corresponding to an address and port.
 *
 * NOTE: This function was taken from Assignment 8
 *
 * Given an address and port number, convert their representations to network
 * byte order and assemble them into an approopriate sockaddr_in. The returned
 * value can then be used to bind to or connect to a socket.
 *
 * @param addr The address to bind or connect the socket to, in host order.
 * @param port The port number to bind or connect the socket to, in host order.
 * @return A sockaddr_in structure to use with bind/connect, in network order.
 */
struct sockaddr_in socket_address(in_addr_t addr, in_port_t port);

FILE* get_socket_file(int socket);

void remove_newline(char* str);

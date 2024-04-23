#pragma once

/**
 * Receive messages from a socket and print them to stdout.
 *
 * Continuously receieves messages from the provided socket, then prints it out
 * to stdout. Terminates if there is an error during receiving or the socket is
 * closed.
 *
 * @param socket_desc A void pointer to the socket descriptor.
 * @returns The Null pointer.
 */

void* receive_message(void* socket_desc);

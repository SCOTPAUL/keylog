#ifndef NETWORKING_H
#define NETWORKING_H

/**
 * Returns the file descriptor of a socket for writing to.
 *
 * If a connection cannot be established, the process is terminated.
 *
 * \param hostname IP address/name of host
 * \param port Port number to connect to on host
 *
 * \returns The file descriptor of a stream socket
 */
int get_socket_file_descriptor(char *hostname, char *port);

/**
 * Returns the file descriptor of a listening socket.
 *
 * If a connection cannot be established, the process is terminated.
 *
 * \param port Port number to listen on
 *
 * \returns The file descriptor of a stream socket
 */
int get_listener_socket_file_descriptor(char *port);

#endif

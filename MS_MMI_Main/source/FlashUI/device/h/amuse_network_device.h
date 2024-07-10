/*
* amuse_network_device.h
* Copyright (C) 2011 Microrapid Inc
* Author: Black <black@Microrapid.com>
* 
* This file contains network interface that need to 
* be implemented on different platforms.
*/
#ifndef _AMUSE_NETWORK_DEVICE_H_
#define _AMUSE_NETWORK_DEVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if __WINSCW__ || __GCCE__
#include <_ansi.h>
#else
#define IMPORT_C
#define EXPORT_C
#endif

typedef unsigned int * amuse_socket_t;
#define AMUSE_INVALID_SOCKET (amuse_socket_t)(~0)

/*
 * This function initiates use of socket.
 *
 * Parameters: none
 *
 * Returns: zero if successful, non-zero means fail
 */
EXPORT_C int amuse_network_socket_startup(void);

/*
 * This function terminates use of socket.
 *
 * Parameters: none
 *
 * Returns: zero if successful, non-zero means fail
 */
EXPORT_C int amuse_network_socket_shutdown(void);

/*
 * Create a new socket.
 *
 * Parameters: 
 * sock : if success, sock is the created socket
 *
 * Returns: 1 success, 0 failed
 */
EXPORT_C amuse_socket_t amuse_network_socket_open();

/*
 * Close socket.
 *
 * Parameters: 
 * sock : the socket to be closed
 *
 * Returns: 1 success, 0 failed
 */
EXPORT_C int amuse_network_socket_close(amuse_socket_t sock);

/*
 * Connect to network.
 *
 * Parameters: 
 * sock: socket object
 * shost: connecting destination.
 * nport: destination port.
 *
 * Returns: 1 success, 0 failed
 */
EXPORT_C int amuse_network_socket_connect(amuse_socket_t sock, char *shost, int nport);

/*
 * Send data to network.
 * Parameters:
 * sock: socket object
 * packet: data buffer
 * buflen: buffer length
 *
 * Returns: sent data length
 */
EXPORT_C int amuse_network_socket_send( amuse_socket_t sock, char *packet, int buflen);

/*
 * Receive fixed length data form network.
 *
 * Parameters:
 * sock: socket object
 * packet: data buffer
 * maxlen: the data length to receive
 * 
 * Returns: received data length, -1 for fail
 */
EXPORT_C int amuse_network_socket_recv(amuse_socket_t sock,char* packet, int maxlen);

/*
 * Set socket option.
 * 
 * Parameters:
 * timeouts: time out (m.seconds)
 * 
 * Returns: 
 */
EXPORT_C int amuse_network_set_nonblock(amuse_socket_t sock, int timeouts);

/*
 * Get error code.
 *
 * Parameters: none
 *
 * Returns: error code 
 */
EXPORT_C int amuse_network_getlasterror(void);

char *amuse_network_get_host_ip(char *hostname);

#ifdef __cplusplus
}
#endif

#endif

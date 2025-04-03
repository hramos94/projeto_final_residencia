#ifndef H_MOCK_CAN_UTILS
#define H_MOCK_CAN_UTILS

#include "mcal.h"
#include <sys/socket.h>

int socket_create(int domain, int type, int protocol);
int socket_close(int can_socket);
int can_ioctl(int fd, unsigned long request, void *args);
int can_bind(int fd, struct sockaddr *addr, socklen_t addrlen);
int can_write(int *can_socket, struct can_frame *frame);
int can_read_socket(int fd, void *buf, size_t count);

#endif

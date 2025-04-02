#ifndef H_CAN_UTILS
#define H_CAN_UTILS
#include "mcal.h"
#include <sys/socket.h>

int can_ioctl(int fd, unsigned long request, void *args);
int can_bind(int fd, struct sockaddr *addr, socklen_t addrlen);
int can_write(int *can_socket, struct can_frame *frame);

#endif

#ifndef H_CAN_UTILS
#define H_CAN_UTILS
#include "mcal.h"

int can_ioctl(int fd, unsigned long request, void *args);
int can_write(int *can_socket, struct can_frame *frame);

#endif

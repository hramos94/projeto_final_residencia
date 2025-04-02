#include <mcal.h>
#include <sys/ioctl.h> // ioctl, SIOCGIFFLAGS, SIOCGIFINDEX
#include <unistd.h>    // close, write

int can_ioctl(int fd, unsigned long request, void *args) { return ioctl(fd, request, args); }

int can_write(int *can_socket, struct can_frame *frame)
{
    return write(*can_socket, frame, sizeof(struct can_frame)) != sizeof(struct can_frame);
}

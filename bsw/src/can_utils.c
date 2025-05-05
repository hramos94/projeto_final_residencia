#include <mcal.h>
#include <sys/ioctl.h> // ioctl, SIOCGIFFLAGS, SIOCGIFINDEX
#include <sys/socket.h> //for socket stuff
#include <unistd.h>    // close, write
#include <can_utils.h>

int socket_create(int domain, int type, int protocol) { return socket(domain, type, protocol); }

int socket_close(int can_socket) { return close(can_socket); }

int can_ioctl(int fd, unsigned long request, void *args) { return ioctl(fd, request, args); }

int can_bind(int fd, struct sockaddr *addr, socklen_t addrlen) { return bind(fd, addr, addrlen); }

int can_write(int *can_socket, struct can_frame *frame)
{
    return (size_t)write(*can_socket, frame, sizeof(struct can_frame)) != sizeof(struct can_frame);
}

int can_read_socket(int fd, void *buf, size_t count) { return read(fd, buf, count); }
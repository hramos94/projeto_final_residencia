#include <sys/ioctl.h> // ioctl, SIOCGIFFLAGS, SIOCGIFINDEX

int can_ioctl(int fd, unsigned long request, void *args) { return ioctl(fd, request, args); }

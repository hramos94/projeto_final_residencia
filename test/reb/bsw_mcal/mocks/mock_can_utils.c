#include <net/if.h> // struct ifreq, IFNAMSIZ

int can_ioctl(int fd, unsigned long request, void *args)
{
    int fake_ioctl_retur_value = 1;
    struct ifreq *ifr = (struct ifreq *)args;
    ifr->ifr_flags = fake_ioctl_retur_value;
    return 0;
}

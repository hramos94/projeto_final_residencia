#include "../../../bsw/inc/ecu.h"
#include "../../../reb/inc/ecu_reb.h"
#include "mcal.h"
#include "stdio.h"
#include <net/if.h> // struct ifreq, IFNAMSIZ

int mock_can_write_return = 0;

int can_ioctl(int fd, unsigned long request, void *args)
{
    int fake_ioctl_retur_value = 1;
    struct ifreq *ifr = (struct ifreq *)args;
    ifr->ifr_flags = fake_ioctl_retur_value;
    return 0;
}

int can_write(int *can_socket, struct can_frame *frame)
{
    if (mock_can_write_return == IPC_REB_START)
    {
        if (frame->data[0] == 0x01 && frame->can_id == REB_IPC_ID)
        {
            return 0;
        }
    }
    return 1;
}

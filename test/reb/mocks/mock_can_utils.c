#include "../../../bsw/inc/ecu.h"
#include "../../../reb/inc/ecu_reb.h"
#include "mcal.h"
#include "stdio.h"
#include <net/if.h> // struct ifreq, IFNAMSIZ
#include <sys/socket.h>

int mock_can_write_return = 0;
int mock_can_read_return = 0;
int mock_can_open_return = 0;
int mock_can_close_return = 0;

int can_ioctl(int fd, unsigned long request, void *args)
{
    if(fd == 1)
    {
        int fake_ioctl_return_value = 1;
        struct ifreq *ifr = (struct ifreq *)args;
        ifr->ifr_flags = fake_ioctl_return_value;
        return 0;
    }
    else if(fd == -1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
    
}
int can_bind(int fd, struct sockaddr *addr, socklen_t addrlen)
{
    if(fd == 0)
    {
        return 0;
    }
    else 
    {
        return -1;
    }
}

int can_write(int *can_socket, struct can_frame *frame)
{
    if (mock_can_write_return == 0)
    {
        mock_can_write_return ++;
        return 0;
        
    }
    else
    {
        mock_can_write_return --;
        return 1;
    }
    
}

int socket_create(int domain, int type, int protocol) 
{
    if(mock_can_open_return == 0)
    {
        mock_can_open_return ++;
        return 1;
    }
    else
    {
        return -1;
    }

}

int socket_close(int can_socket) 
{
    if(mock_can_close_return == 0)
    {
        mock_can_close_return ++;
        return 0;
    }
    else 
    {
        mock_can_close_return --;
        return -1;
    }
}


int can_read_socket(int fd, void *buf, size_t count)
{
    if (mock_can_read_return == 0)
    {
        mock_can_read_return ++;
        return 4;
        
    }
    else
    {
        mock_can_read_return --;
        return -1;
    }
}

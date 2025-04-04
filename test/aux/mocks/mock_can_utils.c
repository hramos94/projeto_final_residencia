#include "../../../bsw/inc/ecu.h"
#include "../../../reb/inc/ecu_reb.h"
#include "mcal.h"
#include <net/if.h> // struct ifreq, IFNAMSIZ
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int reb_ipc_id = 0;
int flag_reb_id = 0;
int flag_can_REB_IPC_count = 0;

void set_reb_ipc_id(int id) { reb_ipc_id = id; }

int mock_can_ioctl_return = 0;
int mock_can_bind_return = 0;
int mock_can_write_return = 0;
int mock_can_read_return = 0;
int mock_can_open_return = 0;
int mock_can_close_return = 0;

/**
 * @brief Define os valores de retorno mockados para chamadas de função.
 *
 * Essa função permite configurar os valores de retorno das funções mockadas
 * para simular diferentes cenários durante os testes.
 *
 * @param ioctl_ret Valor de retorno simulado para ioctl.
 * @param bind_ret Valor de retorno simulado para bind.
 * @param write_ret Valor de retorno simulado para write.
 * @param read_ret Valor de retorno simulado para read.
 * @param open_ret Valor de retorno simulado para open.
 * @param close_ret Valor de retorno simulado para close.
 */
void set_mock_return_values(int ioctl_ret, int bind_ret, int write_ret, int read_ret, int open_ret,
                            int close_ret)
{
    mock_can_ioctl_return = ioctl_ret;
    mock_can_bind_return = bind_ret;
    mock_can_write_return = write_ret;
    mock_can_read_return = read_ret;
    mock_can_open_return = open_ret;
    mock_can_close_return = close_ret;
}

int can_ioctl(int fd, unsigned long request, void *args)
{
    if (mock_can_ioctl_return == 2)
    {
        int fake_ioctl_return_value = 1;
        struct ifreq *ifr = (struct ifreq *)args;
        ifr->ifr_flags = fake_ioctl_return_value;
        return 0;
    }
    else if (mock_can_ioctl_return == 1)
    {
        struct ifreq *ifr = (struct ifreq *)args;
        ifr->ifr_flags = 0; // Interface DOWN
        return 0;
    }
    else
    {
        return -1;
    }
}
int can_bind(int fd, struct sockaddr *addr, socklen_t addrlen)
{
    if (mock_can_bind_return == 0)
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
    if (reb_ipc_id == 1)
    {
        if (frame->data[0] == 0x01 && frame->can_id == reb_ipc_id)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }

    if (flag_reb_id == 1)
    {
        if (frame->data[0] == 0x01 && frame->can_id == TCU_REB_ID)
        {
            flag_can_REB_IPC_count++;
        }
    }

    if (mock_can_write_return == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }

    return 0;
}

int socket_create(int domain, int type, int protocol)
{
    if (mock_can_open_return == 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int socket_close(int can_socket)
{
    if (mock_can_close_return == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int can_read_socket(int fd, void *buf, size_t count)
{
    can_frame frame;

    if (mock_can_read_return == -1)
    {
        sleep(5);
    }
    else if (mock_can_read_return == 0)
    {
        return 4;
    }
    else if (mock_can_read_return == 1)
    {
        mock_can_read_return = -1;
        return -1;
    }
    else if (mock_can_read_return == 2)
    {
        frame.can_id = REB_ECU_ID;
        frame.data[0] = 0x01;
        frame.can_dlc = 8;

        memcpy(buf, &frame, sizeof(frame));

        mock_can_read_return = -1;

        return 0;
    }
    else if (mock_can_read_return == 3)
    {
        frame.can_id = REB_IPC_ID;
        frame.data[0] = 0x01;
        frame.can_dlc = 8;

        memcpy(buf, &frame, sizeof(frame));

        mock_can_read_return = -1;

        return 0;
    }
    else if (mock_can_read_return == 4)
    {
        frame.can_id = 0x015;
        frame.data[0] = 0x10;
        frame.can_dlc = 8;

        memcpy(buf, &frame, sizeof(frame));

        mock_can_read_return = -1;

        return 0;
    }
    else if (mock_can_read_return == 5)
    {
        frame.can_id = 0x015;
        frame.data[0] = 0x11;
        frame.can_dlc = 8;

        memcpy(buf, &frame, sizeof(frame));

        mock_can_read_return = -1;

        return 0;
    }

    frame.can_id = 0x0;
    frame.data[0] = 0x0;
    frame.can_dlc = 0;

    buf = &frame;

    return 0;
}

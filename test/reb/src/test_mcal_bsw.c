#include "mcal.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdlib.h>
#include <stdio.h>


#include  <string.h>
#include <sys/ioctl.h>   // Para SIOCGIFFLAGS
#include <net/if.h>  // For IFF_UP and other interface flags

TEST_GROUP(mcal_can);

extern int Counter;
int vcan_socket = 1;
//const char *interface = "vcan0";

TEST_SETUP(mcal_can)  // This is run before EACH TEST
{
    system("sudo ip link add dev vcan0 type vcan || true");
    system("sudo ip link set up vcan0");
    can_start(&vcan_socket,"vcan0");
}

TEST_TEAR_DOWN(mcal_can)  // This is run after EACH TEST
{
    system("sudo ip link delete vcan0 type vcan || true");

}

TEST(mcal_can, test_can_interface_status_up)
{

    uint8_t result = can_interface_status(&vcan_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(0, result);  // Espera-se sucesso (0)
}

TEST(mcal_can, test_can_interface_status_down)
{
    uint8_t can_status = 0;
    system("sudo ip link set vcan0 down || true");
    can_status = can_interface_status(&vcan_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, can_status);
}

TEST(mcal_can, test_can_interface_status_does_not_exist)
{
    uint8_t can_status = 0;
    system("sudo ip link delete vcan0 type vcan || true");
    can_status = can_interface_status(&vcan_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, can_status);
}


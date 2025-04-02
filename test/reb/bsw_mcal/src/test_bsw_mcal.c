/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "mcal.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(mcal_can);

extern int Counter;

TEST_SETUP(mcal_can) {}

TEST_TEAR_DOWN(mcal_can) {}

TEST(mcal_can, get_status_pin_0)
{
    // All of these should pass
    uint8_t status = 0;
    read_pin_status(&status, 0);

    TEST_ASSERT_EQUAL_INT16(0, status);
}
//tets can interface
TEST(mcal_can, can_interface_status_UP)
{
    int can_socket = 1;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_INT16(0, result);
}
TEST(mcal_can, can_interface_status_DOWN)
{
    int can_socket = 2;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_INT16(1, result);
}

TEST(mcal_can, can_interface_status_DOES_NOT_EXIST)
{
    int can_socket = -1;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_INT16(1, result);
}

// can bind socket
TEST(mcal_can, can_bind_socket_SUCCESS)
{
    int can_socket = 0;
    uint8_t result = can_bind_socket(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_INT16(0, result);
}
TEST(mcal_can, can_bind_socket_FAIL)
{
    int can_socket = 2;
    uint8_t result = can_bind_socket(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_INT16(1, result);
}

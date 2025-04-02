/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "mcal.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(mcal);

// sometimes you may want to get at local data in a module.
// for example: If you plan to pass by reference, this could be useful
// however, it should often be avoided
extern int Counter;

TEST_SETUP(mcal)
{
    // This is run before EACH TEST
}

TEST_TEAR_DOWN(mcal) {}

TEST(mcal, get_status_pin_0)
{
    // All of these should pass
    uint8_t status = 0;
    read_pin_status(&status, 0);

    TEST_ASSERT_EQUAL_INT16(0, status);
}

TEST(mcal, can_interface_status_UP)
{
    int can_socket = 1;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_INT16(0, result);
}
TEST(mcal, can_interface_status_DOWN)
{
    int can_socket = 2;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_INT16(1, result);
}

TEST(mcal, can_interface_status_DOES_NOT_EXIST)
{
    int can_socket = -1;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_INT16(1, result);
}

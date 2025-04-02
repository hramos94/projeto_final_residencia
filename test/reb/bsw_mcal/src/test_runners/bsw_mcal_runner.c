/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(mcal_can)
{
    RUN_TEST_CASE(mcal_can, get_status_pin_0);
    RUN_TEST_CASE(mcal_can, can_interface_status_UP);
    RUN_TEST_CASE(mcal_can, can_interface_status_DOWN);
    RUN_TEST_CASE(mcal_can, can_interface_status_DOES_NOT_EXIST);
    RUN_TEST_CASE(mcal_can, can_bind_socket_SUCCESS);
    RUN_TEST_CASE(mcal_can, can_bind_socket_FAIL);
}

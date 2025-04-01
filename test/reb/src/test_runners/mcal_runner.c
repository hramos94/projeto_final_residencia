/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(mcal) { RUN_TEST_CASE(mcal, get_status_pin_0); }
TEST_GROUP_RUNNER(mcal_can) 
{ 
    RUN_TEST_CASE(mcal_can, test_can_interface_status_up); 
    RUN_TEST_CASE(mcal_can, test_can_interface_status_down);
    RUN_TEST_CASE(mcal_can, test_can_interface_status_does_not_exist);
}

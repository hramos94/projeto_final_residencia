/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(mcal)
{
    RUN_TEST_CASE(mcal, get_status_pin_0);
    RUN_TEST_CASE(mcal, can_interface_status_OK);
}

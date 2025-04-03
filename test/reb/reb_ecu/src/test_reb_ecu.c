/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "ecu_reb.h"
#include "unity.h"
#include "unity_fixture.h"

extern int reb_ipc_id;

TEST_GROUP(reb_ecu);

// sometimes you may want to get at local data in a module.
// for example: If you plan to pass by reference, this could be useful
// however, it should often be avoided

TEST_SETUP(reb_ecu)
{
    // This is run before EACH TEST
}

TEST_TEAR_DOWN(reb_ecu) {}

TEST(reb_ecu, reb_can_send_ipc_status_IPC_REB_START)
{
    reb_ipc_id = 1;
    int result_ok = reb_can_send_ipc(1);
    TEST_ASSERT_EQUAL(0, result_ok);
}

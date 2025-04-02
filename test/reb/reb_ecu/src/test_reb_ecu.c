/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "ecu_reb.h"
#include "unity.h"
#include "unity_fixture.h"

extern int mock_can_write_return;

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
    mock_can_write_return = IPC_REB_START;
    int result_ok = reb_can_send_ipc(IPC_REB_START);
    TEST_ASSERT_EQUAL(0, result_ok);
}

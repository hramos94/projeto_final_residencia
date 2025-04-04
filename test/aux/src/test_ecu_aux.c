/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "ecu.h"
#include "ecu_aux.h"
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(ecu_aux);

// sometimes you may want to get at local data in a module.
// for example: If you plan to pass by reference, this could be useful
// however, it should often be avoided
extern int Counter;

TEST_SETUP(ecu_aux)
{
    // This is run before EACH TEST
}

TEST_TEAR_DOWN(ecu_aux)
{
    // Cleanup after tests
}

TEST(ecu_aux, test_get_hazard_button_status)
{
    uint8_t status = 0;
    TEST_ASSERT_EQUAL_INT16(SUCCESS, get_hazard_button_status(&status));
}

TEST(ecu_aux, test_set_hazard_light)
{
    TEST_ASSERT_EQUAL(SUCCESS, set_hazard_light(1));
    TEST_ASSERT_EQUAL(SUCCESS, set_hazard_light(0));
}

TEST(ecu_aux, test_get_tcu_start_reb)
{
    uint8_t status = 0;
    TEST_ASSERT_EQUAL(SUCCESS, get_tcu_start_reb(&status));
}

TEST(ecu_aux, test_set_tcu_start_reb) { TEST_ASSERT_EQUAL(SUCCESS, set_tcu_start_reb(1)); }

TEST(ecu_aux, test_set_tcu_cancel_reb) { TEST_ASSERT_EQUAL(SUCCESS, set_tcu_cancel_reb(0)); }

TEST(ecu_aux, test_get_tcu_cancel_reb)
{
    uint8_t status = 0;
    TEST_ASSERT_EQUAL(SUCCESS, get_tcu_cancel_reb(&status));
}

TEST(ecu_aux, test_tcu_can_send_reb)
{
    TEST_ASSERT_EQUAL(SUCCESS, tcu_can_send_reb(REB_START));
    TEST_ASSERT_EQUAL(SUCCESS, tcu_can_send_reb(REB_CANCEL));
}

TEST(ecu_aux, test_handle_ecu_can)
{
    unsigned char data[1] = {0x01};
    TEST_ASSERT_EQUAL(SUCCESS, handle_ecu_can(data));

    unsigned char data_unblock[1] = {0x02};
    TEST_ASSERT_EQUAL(SUCCESS, handle_ecu_can(data_unblock));

    // unsigned char invalid_data[1] = {0xFF};
    // TEST_ASSERT_EQUAL(1, handle_ecu_can(invalid_data));
}

TEST(ecu_aux, test_block_engine) { TEST_ASSERT_EQUAL(SUCCESS, block_engine()); }

TEST(ecu_aux, test_unblock_engine) { TEST_ASSERT_EQUAL(SUCCESS, unblock_engine()); }

TEST(ecu_aux, test_handle_ipc_can)
{
    unsigned char data_start[1] = {0x01};
    TEST_ASSERT_EQUAL(SUCCESS, handle_ipc_can(data_start));

    unsigned char data_cancel[1] = {0x02};
    TEST_ASSERT_EQUAL(SUCCESS, handle_ipc_can(data_cancel));
}

TEST(ecu_aux, test_set_reb_warning)
{
    TEST_ASSERT_EQUAL(SUCCESS, set_reb_warning(1));
    TEST_ASSERT_EQUAL(SUCCESS, set_reb_warning(0));
}

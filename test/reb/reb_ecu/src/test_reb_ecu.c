/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "ecu_reb.h"
#include "mock_can_utils.h"
#include "unity.h"
#include "unity_fixture.h"

extern int reb_ipc_id;
int reb_ecu_id;

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
    int result_ok = reb_can_send_ipc(reb_ipc_id);
    TEST_ASSERT_EQUAL(0, result_ok);
}

TEST(reb_ecu, reb_can_send_ipc_status_IPC_REB_CANCEL)
{
    reb_ipc_id = 2;
    int result_ok = reb_can_send_ipc(reb_ipc_id);
    TEST_ASSERT_EQUAL(0, result_ok);
}

TEST(reb_ecu, reb_can_send_ecu_status_ECU_REB_START)
{
    reb_ecu_id = 1;
    int result_ok = reb_can_send_ecu(reb_ecu_id);
    TEST_ASSERT_EQUAL(0, result_ok);
}

TEST(reb_ecu, reb_can_send_ecu_status_ECU_REB_CANCEL)
{
    reb_ecu_id = 2;
    int result_ok = reb_can_send_ecu(reb_ecu_id);
    TEST_ASSERT_EQUAL(0, result_ok);
}

TEST(reb_ecu, reb_can_send_harzard_status_TURN_ON)
{
    uint8_t reb_ecu_harzard_status = 0;
    int result_ok = can_send_hazard_light(reb_ecu_harzard_status);
    TEST_ASSERT_EQUAL(0, result_ok);
}

TEST(reb_ecu, reb_can_send_harzard_status_TURN_OFF)
{
    uint8_t reb_ecu_harzard_status = 2;
    int result_ok = can_send_hazard_light(reb_ecu_harzard_status);
    TEST_ASSERT_EQUAL(0, result_ok);
}

TEST(reb_ecu, reb_handle_tcu_can_CANCEL_REB)
{
    uint8_t data_first_byte_2[8] = {0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int result_ok = handle_tcu_can(data_first_byte_2);
    TEST_ASSERT_EQUAL(0, result_ok);
}

TEST(reb_ecu, reb_handle_tcu_can_START_REB)
{
    uint8_t data_first_byte_1[8] = {0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int result_ok = handle_tcu_can(data_first_byte_1);
    TEST_ASSERT_EQUAL(0, result_ok);
}

TEST(reb_ecu, reb_handle_tcu_can_CANCEL_REB_FAIL)
{
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    uint8_t data_first_byte_2[8] = {0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int result_ok = handle_tcu_can(data_first_byte_2);
    TEST_ASSERT_EQUAL(1, result_ok);
}

TEST(reb_ecu, reb_handle_tcu_can_START_REB_FAIL)
{
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    uint8_t data_first_byte_1[8] = {0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int result_ok = handle_tcu_can(data_first_byte_1);
    TEST_ASSERT_EQUAL(1, result_ok);
}

TEST(reb_ecu, reb_can_send_ecu_CAN_SEND_VCAN_FAIL)
{
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    reb_ecu_id = 2;
    int result_ok = reb_can_send_ecu(reb_ecu_id);
    TEST_ASSERT_EQUAL(1, result_ok);
}

TEST(reb_ecu, reb_can_send_ipc_CAN_SEND_VCAN_FAIL)
{
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    reb_ipc_id = 2;
    int result_ok = reb_can_send_ipc(reb_ipc_id);
    TEST_ASSERT_EQUAL(1, result_ok);
}

TEST(reb_ecu, reb_can_send_harzard_CAN_SEND_VCAN_FAIL)
{
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    uint8_t reb_ecu_harzard_status = 2;
    int result_ok = can_send_hazard_light(reb_ecu_id);
    TEST_ASSERT_EQUAL(1, result_ok);
}

/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

/**
 * @file test_reb_ecu.c
 * @brief Unit tests for functions in ecu_reb.c
 *
 * This file contains the test suite that validates the main functions of
 * Remote Engine Blocking, in ecu_reb, focusing on sending and handling CAN messages.
 * It covers success and failure scenarios, as well as different status/byte values.
 *
 * Requirements covered:
 * - SwHLR_F_3 (handle_tcu_can)
 * - SwHLR_F_8 (reb_can_send_ipc)
 * - SwHLR_F_12 (reb_can_send_ecu)
 */

#include "ecu_reb.h"
#include "mock_can_utils.h"
#include "unity.h"
#include "unity_fixture.h"

extern int flag_fail_set_pin;
extern int reb_ipc_id;
int reb_ecu_id;

TEST_GROUP(reb_ecu);

TEST_SETUP(reb_ecu) 
{
    flag_fail_set_pin = 0;

}

TEST_TEAR_DOWN(reb_ecu) {}

/**
 * @brief Tests reb_can_send_ipc() with IPC_REB_START (1).
 *
 * Scenario:
 *  - reb_ipc_id = 1 (activates REB on the panel)
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SwHLR_F_8}
 */
TEST(reb_ecu, reb_can_send_ipc_status_IPC_REB_START)
{
    reb_ipc_id = 1;
    int result_ok = reb_can_send_ipc(reb_ipc_id);
    TEST_ASSERT_EQUAL(0, result_ok);
}

/**
 * @brief Tests reb_can_send_ipc() with IPC_REB_CANCEL (2).
 *
 * Scenario:
 *  - reb_ipc_id = 2 (deactivates REB on the panel)
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SwHLR_F_8}
 */
TEST(reb_ecu, reb_can_send_ipc_status_IPC_REB_CANCEL)
{
    reb_ipc_id = 2;
    int result_ok = reb_can_send_ipc(reb_ipc_id);
    TEST_ASSERT_EQUAL(0, result_ok);
}

/**
 * @brief Tests reb_can_send_ecu() with ECU_REB_START (1).
 *
 * Scenario:
 *  - reb_ecu_id = 1 (engine block)
 * Expected:
 *  - Return SUCCESS (0).
 *  @requir{SwHLR_F_12}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_2}
 *  @requir{SwHLR_F_1}
 */
TEST(reb_ecu, reb_can_send_ecu_status_ECU_REB_START)
{
    reb_ecu_id = 1;
    int result_ok = reb_can_send_ecu(reb_ecu_id);
    TEST_ASSERT_EQUAL(0, result_ok);
}

/**
 * @brief Tests reb_can_send_ecu() with ECU_REB_CANCEL (2).
 *
 * Scenario:
 *  - reb_ecu_id = 2 (engine unblock)
 * Expected:
 *  - Return SUCCESS (0).
 *  @requir{SwHLR_F_12}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_2}
 *  @requir{SwHLR_F_1}
 */
TEST(reb_ecu, reb_can_send_ecu_status_ECU_REB_CANCEL)
{
    reb_ecu_id = 2;
    int result_ok = reb_can_send_ecu(reb_ecu_id);
    TEST_ASSERT_EQUAL(0, result_ok);
}

/**
 * @brief Tests handle_tcu_can() with the first byte = 0x02 (CANCEL_REB).
 *
 * Scenario:
 *  - data[0] = 0x02
 *  - Expects cancel_reb() to be called internally.
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SwHLR_F_3}
 */
TEST(reb_ecu, reb_handle_tcu_can_CANCEL_REB)
{
    uint8_t data_first_byte_2[8] = {0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int result_ok = handle_tcu_can(data_first_byte_2);
    TEST_ASSERT_EQUAL(0, result_ok);
}

/**
 * @brief Tests handle_tcu_can() with the first byte = 0x01 (START_REB).
 *
 * Scenario:
 *  - data[0] = 0x01
 *  - Expects start_reb() to be called internally.
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SwHLR_F_3}
 */
TEST(reb_ecu, reb_handle_tcu_can_START_REB)
{
    uint8_t data_first_byte_1[8] = {0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int result_ok = handle_tcu_can(data_first_byte_1);
    TEST_ASSERT_EQUAL(0, result_ok);
}

/**
 * @brief Tests handle_tcu_can() for CANCEL_REB when cancel_reb() fails.
 *
 * Scenario:
 *  - data[0] = 0x02
 *  - We force cancel_reb() to fail via mocks.
 * Expected:
 *  - Return FAIL (1).
 * @requir{SwHLR_F_3}
 * @requir{SwHLR_F_1}
 */
TEST(reb_ecu, reb_handle_tcu_can_CANCEL_REB_FAIL)
{
    // Configure the mock function to fail at the position referring to cancel_reb()
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    uint8_t data_first_byte_2[8] = {0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int result_ok = handle_tcu_can(data_first_byte_2);
    TEST_ASSERT_EQUAL(1, result_ok);
}

/**
 * @brief Tests handle_tcu_can() for CANCEL_REB when cancel_reb() and Set_pin_status() fails.
 *
 * Scenario:
 *  - data[0] = 0x02
 *  - We force cancel_reb() to fail via mocks.
 *  - Set_pin_status(S_OFF, REB_COUNTDOWN_PIN) FAIL
 * Expected:
 *  - Return FAIL (1).
 * @requir{SwHLR_F_3}
 * @requir{SwHLR_F_1}
 */
TEST(reb_ecu, reb_handle_tcu_can_CANCEL_REB_SET_PIN_FAIL)
{
    // Configure the mock function to fail at the position referring to cancel_reb()
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    flag_fail_set_pin = 1;
    uint8_t data_first_byte_2[8] = {0x02, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int result_ok = handle_tcu_can(data_first_byte_2);
    TEST_ASSERT_EQUAL(1, result_ok);
}


/**
 * @brief Tests handle_tcu_can() for START_REB when start_reb() fails.
 *
 * Scenario:
 *  - data[0] = 0x01
 *  - We force start_reb() to fail via mocks.
 * Expected:
 *  - Return FAIL (1).
 * @requir{SwHLR_F_3}
 * @requir{SwHLR_F_1}
 */
TEST(reb_ecu, reb_handle_tcu_can_START_REB_FAIL)
{
    // Configure the mock function to fail at the position referring to start_reb()
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    uint8_t data_first_byte_1[8] = {0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int result_ok = handle_tcu_can(data_first_byte_1);
    TEST_ASSERT_EQUAL(1, result_ok);
}

/**
 * @brief Tests handle_tcu_can() for START_REB when start_reb() and set_pin_status() fails.
 *
 * Scenario:
 *  - data[0] = 0x01
 *  - We force start_reb() to fail via mocks.
 *  - set_pin_status(S_ON, REB_COUNTDOWN_PIN) FAIL
 * Expected:
 *  - Return FAIL (1).
 * @requir{SwHLR_F_3}
 * @requir{SwHLR_F_1}
 */
TEST(reb_ecu, reb_handle_tcu_can_START_REB_SET_PIN_FAIL)
{
    // Configure the mock function to fail at the position referring to start_reb()
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    flag_fail_set_pin = 1;
    uint8_t data_first_byte_1[8] = {0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    int result_ok = handle_tcu_can(data_first_byte_1);
    TEST_ASSERT_EQUAL(1, result_ok);
}


/**
 * @brief Tests reb_can_send_ecu() with can_send_vcan0() failure.
 *
 * Scenario:
 *  - reb_ecu_id = 2
 *  - We force can_send_vcan0() to fail via mocks (returning -1).
 * Expected:
 *  - Return FAIL (1).
 *  @requir{SwHLR_F_12}
 *  @requir{SwHLR_F_10}
 *  @requir{SwHLR_F_2}
 *  @requir{SwHLR_F_1}
 */
TEST(reb_ecu, reb_can_send_ecu_CAN_SEND_VCAN_FAIL)
{
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    reb_ecu_id = 2;
    int result_ok = reb_can_send_ecu(reb_ecu_id);
    TEST_ASSERT_EQUAL(1, result_ok);
}

/**
 * @brief Tests reb_can_send_ipc() with can_send_vcan0() failure.
 *
 * Scenario:
 *  - reb_ipc_id = 2
 *  - We force can_send_vcan0() to fail via mocks (returning -1).
 * Expected:
 *  - Return FAIL (1).
 *  @requir{SwHLR_F_8}
 */
TEST(reb_ecu, reb_can_send_ipc_CAN_SEND_VCAN_FAIL)
{
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    reb_ipc_id = 2;
    int result_ok = reb_can_send_ipc(reb_ipc_id);
    TEST_ASSERT_EQUAL(1, result_ok);
}

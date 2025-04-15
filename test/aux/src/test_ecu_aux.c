/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

/**
 * @file test_reb_aux.c
 * @brief Unit tests for functions in ecu_aux.c
 *
 * This file contains the test suite that validates the main functions of
 * Remote Engine Blocking, in ecu_aux, focusing on sending and handling CAN messages.
 * It covers success and failure scenarios, as well as different status/byte values.
 *
 * Requirements covered:
 * - SwHLR_F_3 (handle_tcu_can)
 * - SwHLR_F_8 (reb_can_send_ipc)
 * - SwHLR_F_12 (reb_can_send_ecu)
 */

#define UNITY_STOP_ON_FAIL

#include "ecu.h"
#include "ecu_aux.h"
#include "unity.h"
#include "unity_fixture.h"

extern int flag_fail_get_pin;
extern int flag_fail_set_pin;

TEST_GROUP(ecu_aux);

TEST_SETUP(ecu_aux)
{
    flag_fail_get_pin = 0;
    flag_fail_set_pin = 0;
}

TEST_TEAR_DOWN(ecu_aux)
{
    flag_fail_get_pin = 0;
    flag_fail_set_pin = 0;
}

/**
 * @brief Tests get_hazard_button_status() with status = 0 (SUCCESS)
 *
 * Scenario:
 *  - status = 0 (read if Hazard Button is pressed or not)
 * Expected:
 *  - Return SUCCESS (0).
 */
TEST(ecu_aux, test_get_hazard_button_status)
{
    uint8_t status = 0;
    TEST_ASSERT_EQUAL_INT16(SUCCESS, get_hazard_button_status(&status));
}

/**
 * @brief Tests set_hazard_light() with status = 1 and 0
 *
 * Scenario:
 *  - status = 1 (set Hazard button to HIGH)
 *  - status = 0 (set Hazard button to LOW)
 * Expected:
 *  - Return SUCCESS (0) for both scenarios.
 */
TEST(ecu_aux, test_set_hazard_light)
{
    // Scenario, status = 1
    uint8_t status = 1;
    TEST_ASSERT_EQUAL(SUCCESS, set_hazard_light(1));

    // Scenario, status = 0
    status = 0;
    TEST_ASSERT_EQUAL(SUCCESS, set_hazard_light(0));
}

/**
 * @brief Tests get_tcu_start_reb() with status = 0 (SUCCESS)
 *
 * Scenario:
 *  - status = 0 (read if REB ON Button is pressed or not)
 * Expected:
 *  - Return SUCCESS (0).
 */
TEST(ecu_aux, test_get_tcu_start_reb)
{
    uint8_t status = 0;
    TEST_ASSERT_EQUAL(SUCCESS, get_tcu_start_reb(&status));
}

/**
 * @brief Tests get_tcu_start_reb() with status = 0 (SUCCESS)
 *
 * Scenario:
 *  - status = 0 (read if REB ON Button is pressed or not)
 * Expected:
 *  - Return SUCCESS (0).
 */
TEST(ecu_aux, test_set_tcu_start_reb) { TEST_ASSERT_EQUAL(SUCCESS, set_tcu_start_reb(1)); }

/**
 * @brief Tests set_tcu_cancel_reb() with status = 1 and 0
 *
 * Scenarios:
 *  - status = 1 (set REB OFF Button to HIGH)
 *  - status = 0 (set REB OFF Button to LOW)
 * Expected:
 *  - Return SUCCESS (0) for both scenarios.
 */
TEST(ecu_aux, test_set_tcu_cancel_reb)
{
    // Scenario, status = 1
    uint8_t status = 1;
    TEST_ASSERT_EQUAL(SUCCESS, set_tcu_cancel_reb(status));

    // Scenario, status = 0
    status = 0;
    TEST_ASSERT_EQUAL(SUCCESS, set_tcu_cancel_reb(status));
}

/**
 * @brief Tests set_tcu_cancel_reb() with status = 1 and 0
 *
 * Scenario:
 *  - unable to set_pin_status
 * Expected:
 *  - Return FAIL (1).
 */
TEST(ecu_aux, test_set_tcu_cancel_reb_FAIL)
{
    flag_fail_set_pin = 1;
    TEST_ASSERT_EQUAL(1, set_tcu_cancel_reb(1));
}

/**
 * @brief Tests get_tcu_cancel_reb() with status = 0 (SUCCESS)
 *
 * Scenario:
 *  - status = 0 (read if REB OFF button is pressed or not)
 * Expected:
 *  - Return SUCCESS (0).
 */
TEST(ecu_aux, test_get_tcu_cancel_reb)
{
    uint8_t status = 0;
    TEST_ASSERT_EQUAL(SUCCESS, get_tcu_cancel_reb(&status));
}

/**
 * @brief Tests get_tcu_cancel_reb() with FAIL return
 *
 * Scenario:
 *  - unable to set_pin_status
 * Expected:
 *  - Return FAIL (1).
 */
TEST(ecu_aux, test_get_tcu_cancel_reb_FAIL)
{
    uint8_t status = 0;
    flag_fail_get_pin = 1;
    TEST_ASSERT_EQUAL(1, get_tcu_cancel_reb(&status));
}

/**
 * @brief Tests tcu_can_send_reb() with Status 1 and 2.
 *
 * Scenarios:
 *  - REB_START = 0x01 - to send frame data 0x01(ON)
 *  - REB_CANCEL = 0x02 - to send frame data 0x02(OFF)
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SwHLR_F_8}
 */
TEST(ecu_aux, test_tcu_can_send_reb)
{
    // Scenario, REB_START (status = 1)
    TEST_ASSERT_EQUAL(SUCCESS, tcu_can_send_reb(REB_START));

    // Scenario, REB_CANCEL (status = 2)
    TEST_ASSERT_EQUAL(SUCCESS, tcu_can_send_reb(REB_CANCEL));
}

/**
 * @brief Tests if handle messages received from REB to Egine Control Unit
 *
 * Scenarios:
 *  - signalREB = 0x01 - to send frame data 0x01(ON)
 *  - signalREB = 0x02 - to send frame data 0x02(OFF)
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SwHLR_F_3}
 */
TEST(ecu_aux, test_handle_ecu_can)
{
    unsigned char data[1] = {0x01};
    TEST_ASSERT_EQUAL(SUCCESS, handle_ecu_can(data));

    unsigned char data_unblock[1] = {0x02};
    TEST_ASSERT_EQUAL(SUCCESS, handle_ecu_can(data_unblock));
}

/**
 * @brief Tests test_handle_ecu_can_block() with FAIL return
 *
 * Scenario:
 *  - unable to set_pin_status
 * Expected:
 *  - Return FAIL (1).
 */
TEST(ecu_aux, test_handle_ecu_can_block_FAIL)
{
    unsigned char data[1] = {0x01};
    flag_fail_set_pin = 1;
    TEST_ASSERT_EQUAL(1, handle_ecu_can(data));
}

/**
 * @brief Tests test_handle_ecu_can_unblock() with FAIL return
 *
 * Scenario:
 *  - unable to set_pin_status
 * Expected:
 *  - Return FAIL (1).
 */
TEST(ecu_aux, test_handle_ecu_can_unblock_FAIL)
{
    unsigned char data[1] = {0x02};
    flag_fail_set_pin = 1;
    TEST_ASSERT_EQUAL(1, handle_ecu_can(data));
}

/**
 * @brief Tests iniciate the engine block
 *
 * Scenario:
 *  - call of function
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SwHLR_F_12}
 */
TEST(ecu_aux, test_block_engine) { TEST_ASSERT_EQUAL(SUCCESS, block_engine()); }

/**
 * @brief Tests test_block_engine() with FAIL return
 *
 * Scenario:
 *  - unable to set_pin_status
 * Expected:
 *  - Return FAIL (1).
 */
TEST(ecu_aux, test_block_engine_FAIL)
{
    flag_fail_set_pin = 1;
    TEST_ASSERT_EQUAL(1, block_engine());
}

/**
 * @brief Tests iniciate engine unblock
 *
 * Scenario:
 *  - call of function
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SwHLR_F_12}
 */
TEST(ecu_aux, test_unblock_engine) { TEST_ASSERT_EQUAL(SUCCESS, unblock_engine()); }

/**
 * @brief Tests test_unblock_engine() with FAIL return
 *
 * Scenario:
 *  - unable to set_pin_status
 * Expected:
 *  - Return FAIL (1).
 */
TEST(ecu_aux, test_unblock_engine_FAIL)
{
    flag_fail_set_pin = 1;
    TEST_ASSERT_EQUAL(1, unblock_engine());
}

/**
 * @brief Tests if handle messages received from REB to instrument painel control
 *
 * Scenarios:
 *  - signalREB = 0x01 - signal of REB = ON
 *  - signalREB = 0x02 - signal of REB = OFF
 * Expected:
 *  - Return SUCCESS (0).
 */
TEST(ecu_aux, test_handle_ipc_can)
{
    unsigned char data_start[1] = {0x01};
    TEST_ASSERT_EQUAL(SUCCESS, handle_ipc_can(data_start));

    unsigned char data_cancel[1] = {0x02};
    TEST_ASSERT_EQUAL(SUCCESS, handle_ipc_can(data_cancel));
}

/**
 * @brief Tests of change status of REB Warning of IPC
 *
 * Scenarios:
 *  - status = 1 Pointer to store the status Button ON
 *  - status = 0 Pointer to store the status Button OFF
 * Expected:
 *  - Return SUCCESS (0).
 */
TEST(ecu_aux, test_set_reb_warning)
{
    uint8_t status = 1;
    TEST_ASSERT_EQUAL(SUCCESS, set_reb_warning(status));
    status = 0;
    TEST_ASSERT_EQUAL(SUCCESS, set_reb_warning(status));
}

/**
 * @brief Tests test_set_reb_warning_() with FAIL return
 *
 * Scenario:
 *  - unable to set_pin_status
 * Expected:
 *  - Return FAIL (1).
 */
TEST(ecu_aux, test_set_reb_warning_FAIL)
{
    uint8_t status = 1;
    flag_fail_set_pin = 1;
    TEST_ASSERT_EQUAL(1, set_reb_warning(status));
}

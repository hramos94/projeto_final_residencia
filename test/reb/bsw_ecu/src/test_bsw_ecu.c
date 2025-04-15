/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

/**
 * @file ecu.c
 * @brief Unit tests for functions in ecu.c
 *
 * This file contains the test suite that validates the main functions of
 * Base Software for ECU
 * It covers success and failure scenarios, as well as different status/byte values.
 *
 */

#include "ecu.h"
#include "mcal.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdint.h>
#include <string.h>

extern int flag_fail_set_pin;

TEST_GROUP(bsw_ecu);

TEST_SETUP(bsw_ecu) {}

TEST_TEAR_DOWN(bsw_ecu) {}

/**
 * @brief Tests for read_console function
 *
 * Scenario:
 *  - User write pin 1 1 into the terminal.
 * Expected:
 *  - Return SUCCESS (0).
 */
TEST(bsw_ecu, read_pint_status_valid_input)
{
    char input_data[] = "pin 1 1\n";
    FILE *mock_stdin = fmemopen(input_data, strlen(input_data), "r");

    FILE *original_stdin = stdin;
    stdin = mock_stdin;

    uint8_t status;
    read_console();
    read_pin_status(&status, 1);

    stdin = original_stdin;
    fclose(mock_stdin);

    TEST_ASSERT_EQUAL_UINT(1, status);
}

/**
 * @brief Tests for read_console, setting status diff from 0 or 1.
 *
 * Scenario:
 *  - User write pin 3 3 into the terminal.
 * Expected:
 *  - Return FAIL (0).
 */
TEST(bsw_ecu, read_pint_status_invalid_input)
{
    char input_data[] = "pin 3 3\n";
    FILE *mock_stdin = fmemopen(input_data, strlen(input_data), "r");

    FILE *original_stdin = stdin;
    stdin = mock_stdin;

    uint8_t result = 0;
    result = read_console();

    stdin = original_stdin;
    fclose(mock_stdin);

    TEST_ASSERT_EQUAL_UINT(1, result);
}

/**
 * @brief Tests for read_console function, setting when a pin is unavailable.
 *
 * Scenario:
 *  - User write pin 3 0 into the terminal.
 * Expected:
 *  - Return FAIL (1).
 */
TEST(bsw_ecu, read_pint_status_invalid_erro_set_pin)
{
    flag_fail_set_pin = 1;

    char input_data[] = "pin 3 0\n";
    FILE *mock_stdin = fmemopen(input_data, strlen(input_data), "r");

    FILE *original_stdin = stdin;
    stdin = mock_stdin;

    uint8_t result = 0;
    result = read_console();

    stdin = original_stdin;
    fclose(mock_stdin);

    TEST_ASSERT_EQUAL_UINT(1, result);
}

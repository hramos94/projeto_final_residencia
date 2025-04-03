/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "ecu.h"
#include "mcal.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdint.h>
#include <string.h>

extern int flag_fail_set_pin;

TEST_GROUP(bsw_ecu);

// sometimes you may want to get at local data in a module.
// for example: If you plan to pass by reference, this could be useful
// however, it should often be avoided

TEST_SETUP(bsw_ecu)
{
    // This is run before EACH TEST
}

TEST_TEAR_DOWN(bsw_ecu) {}

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

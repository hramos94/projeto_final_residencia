/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(mcal_can)
{
    RUN_TEST_CASE(mcal_can, can_interface_status_UP);
    RUN_TEST_CASE(mcal_can, can_interface_status_DOWN);
    RUN_TEST_CASE(mcal_can, can_interface_status_DOES_NOT_EXIST);
    RUN_TEST_CASE(mcal_can, can_bind_socket_SUCCESS);
    RUN_TEST_CASE(mcal_can, can_bind_socket_FAIL);
    RUN_TEST_CASE(mcal_can, can_socket_open_SUCCESS);
    RUN_TEST_CASE(mcal_can, can_socket_open_FAIL);
    RUN_TEST_CASE(mcal_can, can_socket_close_SUCCESS);
    RUN_TEST_CASE(mcal_can, can_socket_close_FAIL);
    RUN_TEST_CASE(mcal_can, can_close_SUCCESS);
    RUN_TEST_CASE(mcal_can, can_send_SUCCESS);
    RUN_TEST_CASE(mcal_can, can_send_FAIL);
    RUN_TEST_CASE(mcal_can, can_send_vcan0_SUCCESS);
    RUN_TEST_CASE(mcal_can, can_read_SUCCESS);
    RUN_TEST_CASE(mcal_can, can_read_FAIL);
    RUN_TEST_CASE(mcal_can, can_read_vcan0_SUCCESS);
    RUN_TEST_CASE(mcal_can, can_start_SUCCESS);
    RUN_TEST_CASE(mcal_can, can_start_SOCKET_FAIL);
    RUN_TEST_CASE(mcal_can, can_start_INTERFACE_FAIL);
    RUN_TEST_CASE(mcal_can, can_start_BIND_FAIL);
    RUN_TEST_CASE(mcal_can, can_init_SUCCESS);
}

TEST_GROUP_RUNNER(mcal_others)
{
    RUN_TEST_CASE(mcal_others, read_pin_status_FAIL_PIN_NUMBER);
    RUN_TEST_CASE(mcal_others, read_pin_status_SUCCESS);
    RUN_TEST_CASE(mcal_others, set_pin_status_FAIL_PIN_NUMBER);
    RUN_TEST_CASE(mcal_others, set_pin_status_SUCCESS);
    RUN_TEST_CASE(mcal_others, read_pint_status_SUCCESS_STATUS_1);
    RUN_TEST_CASE(mcal_others, read_pint_status_FAIL_GETLINE);
    RUN_TEST_CASE(mcal_others, read_pint_status_FAIL_WRONG_PREFIX);
    RUN_TEST_CASE(mcal_others, read_pint_status_FAIL_INVALID_STATUS);
    RUN_TEST_CASE(mcal_others, read_pint_status_FAIL_SSCANF);
    RUN_TEST_CASE(mcal_others, read_pint_status_FAIL_EDGE_CASE);
    RUN_TEST_CASE(mcal_others, read_pint_status_SUCCESS_STATUS_0);
    RUN_TEST_CASE(mcal_others, mcal_init_SUCCESS);
    RUN_TEST_CASE(mcal_others, go_sleep_1);
    RUN_TEST_CASE(mcal_others, show_error_TEXT);
    RUN_TEST_CASE(mcal_others, new_thread_SUCCESS);
}

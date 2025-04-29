/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(reb_app) { 
    RUN_TEST_CASE(reb_app, read_console);
    RUN_TEST_CASE(reb_app, read_console_fail);
    RUN_TEST_CASE(reb_app, application_init_OK); 
    RUN_TEST_CASE(reb_app, application_init_FAIL);
    RUN_TEST_CASE(reb_app, cancel_reb_success);
    RUN_TEST_CASE(reb_app, cancel_reb_fail_ipc);
    RUN_TEST_CASE(reb_app, cancel_reb_fail_ecu);
    RUN_TEST_CASE(reb_app, start_reb_success);
    RUN_TEST_CASE(reb_app, start_reb_fail_send_can);
    RUN_TEST_CASE(reb_app, monitor_read_can_get_handle_tcu);
    RUN_TEST_CASE(reb_app, monitor_read_can_get_handle_tcu_fail);
    RUN_TEST_CASE(reb_app, monitor_read_can_check_fail);
    RUN_TEST_CASE(reb_app, monitor_read_can_check_REB_AUX_comunication);
    RUN_TEST_CASE(reb_app, monitor_read_can_check_WRONG_FRAME);
    RUN_TEST_CASE(reb_app, monitor_read_can_CAN_SEND_FAIL);
    RUN_TEST_CASE(reb_app, countdown_reb_not_inicialize);
    RUN_TEST_CASE(reb_app, countdown_reb_not_inicialize_READ_PIN_FAIL);
    RUN_TEST_CASE(reb_app, countdown_reb_inicialize);
    RUN_TEST_CASE(reb_app, countdown_reb_inicialize_CAN_FAIL);
    }

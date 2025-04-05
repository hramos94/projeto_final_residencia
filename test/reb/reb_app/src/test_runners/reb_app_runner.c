/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(reb_app) { 
    RUN_TEST_CASE(reb_app, test_read_console_pin_01);
    RUN_TEST_CASE(reb_app, application_init_OK); 
    RUN_TEST_CASE(reb_app, application_init_FAIL);
    RUN_TEST_CASE(reb_app, cancel_reb_success);
    RUN_TEST_CASE(reb_app, cancel_reb_fail_ipc);
    RUN_TEST_CASE(reb_app, start_reb_success);
    RUN_TEST_CASE(reb_app, start_reb_fail_send_can);
    RUN_TEST_CASE(reb_app, start_reb_was_canceled_before_timeout);
    }

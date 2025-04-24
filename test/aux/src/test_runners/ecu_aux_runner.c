/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity_fixture.h"

TEST_GROUP_RUNNER(ecu_aux)
{
    RUN_TEST_CASE(ecu_aux, test_get_hazard_button_status);
    RUN_TEST_CASE(ecu_aux, test_set_hazard_light);
    RUN_TEST_CASE(ecu_aux, test_get_tcu_start_reb);
    RUN_TEST_CASE(ecu_aux, test_set_tcu_start_reb);
    RUN_TEST_CASE(ecu_aux, test_set_tcu_cancel_reb);
    RUN_TEST_CASE(ecu_aux, test_get_tcu_cancel_reb);
    RUN_TEST_CASE(ecu_aux, test_tcu_can_send_reb);
    RUN_TEST_CASE(ecu_aux, test_handle_ecu_can);
    RUN_TEST_CASE(ecu_aux, test_block_engine);
    RUN_TEST_CASE(ecu_aux, test_unblock_engine);
    RUN_TEST_CASE(ecu_aux, test_handle_ipc_can);
    RUN_TEST_CASE(ecu_aux, test_handle_ipc_can_S_ON_FAILED);
    RUN_TEST_CASE(ecu_aux, test_handle_ipc_can_S_OFF_FAILED);
    RUN_TEST_CASE(ecu_aux, test_set_reb_warning);
    RUN_TEST_CASE(ecu_aux, test_set_tcu_cancel_reb_FAIL);
    RUN_TEST_CASE(ecu_aux, test_block_engine_FAIL);
    RUN_TEST_CASE(ecu_aux, test_block_engine_ENGINE_REB_MODE_FAIL);
    RUN_TEST_CASE(ecu_aux, test_block_engine_REB_IPC_WARNING_FAIL);
    RUN_TEST_CASE(ecu_aux, test_unblock_engine_FAIL);
    RUN_TEST_CASE(ecu_aux, test_unblock_engine_ENGINE_REB_MODE_FAIL);
    RUN_TEST_CASE(ecu_aux, test_unblock_engine_REB_IPC_WARNING_FAIL);
    RUN_TEST_CASE(ecu_aux, test_set_reb_warning_FAIL);
    RUN_TEST_CASE(ecu_aux, test_handle_ecu_can_block_FAIL);
    RUN_TEST_CASE(ecu_aux, test_handle_ecu_can_unblock_FAIL);
}

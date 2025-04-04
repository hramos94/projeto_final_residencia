/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(reb_ecu)
{
    RUN_TEST_CASE(reb_ecu, reb_can_send_ipc_status_IPC_REB_START);
    RUN_TEST_CASE(reb_ecu, reb_can_send_ipc_status_IPC_REB_CANCEL);
    RUN_TEST_CASE(reb_ecu, reb_can_send_ecu_status_ECU_REB_START);
    RUN_TEST_CASE(reb_ecu, reb_can_send_ecu_status_ECU_REB_CANCEL);
    RUN_TEST_CASE(reb_ecu, reb_can_send_harzard_status_TURN_ON);
    RUN_TEST_CASE(reb_ecu, reb_can_send_harzard_status_TURN_OFF);
    RUN_TEST_CASE(reb_ecu, reb_handle_tcu_can_CANCEL_REB);
    RUN_TEST_CASE(reb_ecu, reb_handle_tcu_can_START_REB);
    RUN_TEST_CASE(reb_ecu, reb_handle_tcu_can_CANCEL_REB_FAIL);
    RUN_TEST_CASE(reb_ecu, reb_handle_tcu_can_START_REB_FAIL);
    RUN_TEST_CASE(reb_ecu, reb_can_send_ecu_CAN_SEND_VCAN_FAIL);
    RUN_TEST_CASE(reb_ecu, reb_can_send_ipc_CAN_SEND_VCAN_FAIL);
    RUN_TEST_CASE(reb_ecu, reb_can_send_harzard_CAN_SEND_VCAN_FAIL);
}

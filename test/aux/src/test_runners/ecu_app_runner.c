#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(ecu_app)
{
    RUN_TEST_CASE(ecu_app, application_init_OK);
    RUN_TEST_CASE(ecu_app, application_init_FAIL);
    RUN_TEST_CASE(ecu_app, read_input_sucess);
    RUN_TEST_CASE(ecu_app, read_input_fail);
    RUN_TEST_CASE(ecu_app, hazard_lights_blink_BUTTON_OFF);
    RUN_TEST_CASE(ecu_app, hazard_lights_blink_BUTTON_ON);
    RUN_TEST_CASE(ecu_app, hazard_lights_blink_get_hazard_button_FAIL);
    RUN_TEST_CASE(ecu_app, hazard_lights_blink_set_hazard_light_ON_FAIL);
    RUN_TEST_CASE(ecu_app, hazard_lights_blink_set_hazard_light_OFF_FAIL);
    RUN_TEST_CASE(ecu_app, monitor_read_can_get_handle_ecu_can);
    RUN_TEST_CASE(ecu_app, monitor_reac_can_get_handle_ipc_can);
    RUN_TEST_CASE(ecu_app, monitor_read_can_get_handle_REB_AUX_communication);
    RUN_TEST_CASE(ecu_app, monitor_read_can_get_handle_REB_AUX_communication_Diff_data_0x02);
    RUN_TEST_CASE(ecu_app, monitor_read_can_get_handle_FAIL_CAN);
    RUN_TEST_CASE(ecu_app, monitor_tcu_set_reb_start_button);
    RUN_TEST_CASE(ecu_app, monitor_tcu_set_reb_cancel_button);
    RUN_TEST_CASE(ecu_app, monitor_tcu_get_reb_button_FAIL);
    RUN_TEST_CASE(ecu_app, monitor_tcu_get_set_button_FAIL);
    RUN_TEST_CASE(ecu_app, monitor_tcu_can_send_reb_FAIL);
    RUN_TEST_CASE(ecu_app, check_can_communication_SEND_OK_RECEIVE_OK);
    RUN_TEST_CASE(ecu_app, check_can_communication_SEND_OK_RECEIVE_FAULT);
    RUN_TEST_CASE(ecu_app, check_can_communication_SEND_CAN_FAIL);
}

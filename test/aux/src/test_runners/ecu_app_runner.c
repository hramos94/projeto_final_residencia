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
}

/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "app.h"
#include "ecu.h"
#include "ecu_aux.h"
#include "mcal.h"
#include "pins.h"
#include "unity.h"
#include "unity_fixture.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int mock_can_open_return;
extern int mock_can_ioctl_return;
extern int flag_fail_set_pin;
extern int flag_fail_get_pin;
extern int flag_cout_set_pin[10];
extern int flag_status_pin[10];
extern int mock_can_read_return;
extern int mock_can_write_return;

extern uint8_t reb_con;
extern int flag_can_REB_IPC_count;
extern int flag_reb_id;

TEST_GROUP(ecu_app);

TEST_SETUP(ecu_app)
{

    for (int i = 0; i < 10; i++)
    {
        flag_cout_set_pin[i] = 0;
        flag_status_pin[i] = 0;
    }

    mock_can_ioctl_return = 0;
    mock_can_open_return = 0;
    flag_fail_set_pin = 0;
    flag_fail_get_pin = 0;
    mock_can_read_return = 0;
    mock_can_write_return = 0;

    reb_con = 0;
    flag_can_REB_IPC_count = 0;
    flag_reb_id = 0;
}

TEST_TEAR_DOWN(ecu_app)
{
    // Cleanup after tests
}

TEST(ecu_app, application_init_OK)
{
    uint8_t status = 0;
    mock_can_open_return = 0;
    mock_can_ioctl_return = 2;
    status = application_init();
    TEST_ASSERT_EQUAL_INT(SUCCESS, status);
}

TEST(ecu_app, application_init_FAIL)
{
    uint8_t status = 0;
    mock_can_ioctl_return = 2;
    mock_can_open_return = 1;
    status = application_init();
    TEST_ASSERT_EQUAL_INT(FAIL, status);
}

TEST(ecu_app, read_input_sucess)
{
    mcal_init();

    pthread_t th_read_input;
    pthread_create(&th_read_input, NULL, (void *)read_input, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    int pipefd[2];
    pipe(pipefd);

    char input[] = "pin 1 1\n";

    write(pipefd[1], input, strlen(input));

    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    uint8_t status = 0, result = 0;
    result = read_pin_status(&status, 1);

    pthread_cancel(th_read_input);

    TEST_ASSERT_EQUAL_INT(SUCCESS, status);
    TEST_ASSERT_EQUAL_INT(SUCCESS, result);
}

TEST(ecu_app, read_input_fail)
{
    flag_fail_set_pin = 1;

    mcal_init();

    pthread_t th_read_input;
    pthread_create(&th_read_input, NULL, (void *)read_input, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    int pipefd[2];
    pipe(pipefd);

    char input[] = "pin 1 1\n";

    write(pipefd[1], input, strlen(input));

    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    uint8_t status = 0;
    status = flag_status_pin[1];

    sleep(1);
    pthread_cancel(th_read_input);

    // Status should stay 0;
    TEST_ASSERT_EQUAL_INT(0, status);
}

TEST(ecu_app, hazard_lights_blink_BUTTON_OFF)
{
    mcal_init();

    pthread_t th_hazard_lights_blink;
    pthread_create(&th_hazard_lights_blink, NULL, (void *)hazard_lights_blink, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(3);
    pthread_cancel(th_hazard_lights_blink);

    TEST_ASSERT_EQUAL(0, flag_cout_set_pin[HAZARD_BUTTON_PIN]);
}

TEST(ecu_app, hazard_lights_blink_BUTTON_ON)
{
    mcal_init();

    pthread_t th_hazard_lights_blink;
    pthread_create(&th_hazard_lights_blink, NULL, (void *)hazard_lights_blink, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    uint8_t status = 0;
    status = set_pin_status(1, HAZARD_BUTTON_PIN);

    TEST_ASSERT_EQUAL(0, status);

    sleep(3);
    pthread_cancel(th_hazard_lights_blink);

    TEST_ASSERT_GREATER_THAN(0, flag_cout_set_pin[HAZARD_BUTTON_PIN]);
    TEST_ASSERT_GREATER_THAN(0, flag_cout_set_pin[HAZARD_LIGHTS_PIN]);
}

TEST(ecu_app, hazard_lights_blink_get_hazard_button_FAIL)
{

    mcal_init();

    uint8_t status = 0;
    status = set_pin_status(1, HAZARD_BUTTON_PIN);
    TEST_ASSERT_EQUAL(0, status);

    // at this points, reading pins throw errors.
    flag_fail_get_pin = 1;

    pthread_t th_hazard_lights_blink;
    pthread_create(&th_hazard_lights_blink, NULL, (void *)hazard_lights_blink, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);
    pthread_cancel(th_hazard_lights_blink);

    TEST_ASSERT_EQUAL(1, flag_cout_set_pin[HAZARD_BUTTON_PIN]);
    // Error for reading pins, so should not blink lights
    TEST_ASSERT_EQUAL(0, flag_cout_set_pin[HAZARD_LIGHTS_PIN]);
}

TEST(ecu_app, hazard_lights_blink_set_hazard_light_ON_FAIL)
{

    mcal_init();

    uint8_t status = 0;
    status = set_pin_status(1, HAZARD_BUTTON_PIN);
    TEST_ASSERT_EQUAL(0, status);

    // at this points, reading pins throw errors.
    flag_fail_set_pin = 1;

    pthread_t th_hazard_lights_blink;
    pthread_create(&th_hazard_lights_blink, NULL, (void *)hazard_lights_blink, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);
    pthread_cancel(th_hazard_lights_blink);

    TEST_ASSERT_EQUAL(1, flag_cout_set_pin[HAZARD_BUTTON_PIN]);
    // Error for seting pins, so should not blink lights
    TEST_ASSERT_EQUAL(0, flag_cout_set_pin[HAZARD_LIGHTS_PIN]);
}

TEST(ecu_app, hazard_lights_blink_set_hazard_light_OFF_FAIL)
{

    mcal_init();

    uint8_t status = 0;
    status = set_pin_status(1, HAZARD_BUTTON_PIN);
    TEST_ASSERT_EQUAL(0, status);

    pthread_t th_hazard_lights_blink;
    pthread_create(&th_hazard_lights_blink, NULL, (void *)hazard_lights_blink, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    while (flag_status_pin[HAZARD_LIGHTS_PIN] == 0)
    {
    };
    // at this points, reading pins throw errors when Lights is ON
    flag_fail_set_pin = 1;

    sleep(2);

    pthread_cancel(th_hazard_lights_blink);

    TEST_ASSERT_EQUAL(1, flag_cout_set_pin[HAZARD_BUTTON_PIN]);
    // Error for seting pins after is ON, so should only Turn ON
    TEST_ASSERT_EQUAL(1, flag_cout_set_pin[HAZARD_LIGHTS_PIN]);
}

TEST(ecu_app, monitor_read_can_get_handle_ecu_can)
{

    mock_can_read_return = 2;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    // If received the ecu_can, should turOn the Hazard light
    TEST_ASSERT_EQUAL(1, flag_cout_set_pin[HAZARD_BUTTON_PIN]);
    TEST_ASSERT_EQUAL(1, flag_cout_set_pin[ENGINE_REB_MODE]);
    TEST_ASSERT_EQUAL(1, flag_cout_set_pin[REB_IPC_WARNING]);
}

TEST(ecu_app, monitor_reac_can_get_handle_ipc_can)
{

    mock_can_read_return = 3;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    // Check if warning REB is ON;
    TEST_ASSERT_EQUAL(1, flag_cout_set_pin[REB_IPC_WARNING]);
}

TEST(ecu_app, monitor_read_can_get_handle_REB_AUX_communication)
{

    mock_can_read_return = 4;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    // Check if reb_con variable from app.c is changed to 1
    TEST_ASSERT_EQUAL(1, reb_con);
}

TEST(ecu_app, monitor_read_can_get_handle_REB_AUX_communication_Diff_data_0x02)
{

    mock_can_read_return = 5;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    // Check if reb_con variable from app.c is not changed to 1
    TEST_ASSERT_EQUAL(0, reb_con);
}

TEST(ecu_app, monitor_read_can_get_handle_FAIL_CAN)
{

    mock_can_read_return = 1;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    // Nothing has been changed
    TEST_ASSERT_EQUAL(0, reb_con);
    TEST_ASSERT_EQUAL(0, flag_cout_set_pin[HAZARD_BUTTON_PIN]);
    TEST_ASSERT_EQUAL(0, flag_cout_set_pin[ENGINE_REB_MODE]);
    TEST_ASSERT_EQUAL(0, flag_cout_set_pin[REB_IPC_WARNING]);
}

TEST(ecu_app, monitor_tcu_set_reb_start_button)
{

    flag_reb_id = 1;
    uint8_t status = 0;
    status = set_pin_status(1, REB_ACTIVATE_PIN);

    TEST_ASSERT_EQUAL(0, status);

    pthread_t th_monitor_tcu;
    pthread_create(&th_monitor_tcu, NULL, (void *)monitor_tcu, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_tcu);

    // Shoud send signal and returned button to 0
    TEST_ASSERT_EQUAL(0, flag_status_pin[REB_ACTIVATE_PIN]);
    TEST_ASSERT_GREATER_THAN(0, flag_can_REB_IPC_count);
}

TEST(ecu_app, monitor_tcu_set_reb_cancel_button)
{

    flag_reb_id = 1;
    uint8_t status = 0;
    status = set_pin_status(1, REB_DEACTIVATE);

    TEST_ASSERT_EQUAL(0, status);

    pthread_t th_monitor_tcu;
    pthread_create(&th_monitor_tcu, NULL, (void *)monitor_tcu, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_tcu);

    // Shoud send signal and returned button to 0
    TEST_ASSERT_EQUAL(0, flag_status_pin[REB_DEACTIVATE]);
    TEST_ASSERT_GREATER_THAN(0, flag_can_REB_IPC_count);
}

TEST(ecu_app, monitor_tcu_get_reb_button_FAIL)
{

    uint8_t status = 0;
    status = set_pin_status(1, REB_ACTIVATE_PIN);

    TEST_ASSERT_EQUAL(0, status);

    flag_fail_get_pin = 1;

    pthread_t th_monitor_tcu;
    pthread_create(&th_monitor_tcu, NULL, (void *)monitor_tcu, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_tcu);

    // Shoud not send signal ;
    TEST_ASSERT_EQUAL(0, flag_can_REB_IPC_count);
}

TEST(ecu_app, monitor_tcu_get_set_button_FAIL)
{

    uint8_t status = 0;

    status = set_pin_status(1, REB_ACTIVATE_PIN);
    TEST_ASSERT_EQUAL(0, status);

    flag_fail_set_pin = 1;

    pthread_t th_monitor_tcu;
    pthread_create(&th_monitor_tcu, NULL, (void *)monitor_tcu, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_tcu);

    // Shoud not send signal ;
    TEST_ASSERT_EQUAL(0, flag_can_REB_IPC_count);
    // should stay ON, error trying to SET 0
    TEST_ASSERT_EQUAL(1, flag_status_pin[REB_ACTIVATE_PIN]);
}

TEST(ecu_app, monitor_tcu_can_send_reb_FAIL)
{

    uint8_t status = 0;

    status = set_pin_status(1, REB_ACTIVATE_PIN);
    TEST_ASSERT_EQUAL(0, status);

    mock_can_write_return = 1;

    pthread_t th_monitor_tcu;
    pthread_create(&th_monitor_tcu, NULL, (void *)monitor_tcu, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_tcu);

    // Shoud not send signal ;
    TEST_ASSERT_EQUAL(0, flag_can_REB_IPC_count);
    // should turned off
    TEST_ASSERT_EQUAL(0, flag_status_pin[REB_ACTIVATE_PIN]);
}

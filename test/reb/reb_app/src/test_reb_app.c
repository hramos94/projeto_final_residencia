/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "ecu.h"
#include "pins.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ecu_reb.h"
#include "unity.h"
#include "unity_fixture.h"
#include "app.h"
#include "pthread.h"
#include "string.h"
#include "mcal.h"
#include "mock_can_utils.h"
#include <net/if.h> // struct ifreq, IFNAMSIZ
#include <string.h>
#include <sys/socket.h>

extern int mock_can_write_return;
extern int mock_can_open_return;
extern int mock_can_ioctl_return;
extern int flag_fail_set_pin;
extern int flag_fail_get_pin;
extern int flag_cout_set_pin[10];
extern int flag_status_pin[10];
extern int mock_can_read_return;
extern int flag_send_ecu;
extern int flag_send_ecu_count;

extern uint8_t read_input();  
extern uint8_t flag_reb_canceled;



TEST_GROUP(reb_app);

TEST_SETUP(reb_app)
{

    mcal_init();

    for (int i = 0; i < 10; i++)
    {
        flag_cout_set_pin[i] = 0;
        flag_status_pin[i] = 0;
    }

    mock_can_write_return = 0;
    mock_can_read_return = 0;
    mock_can_ioctl_return = 0;
    mock_can_open_return = 0;
    flag_fail_set_pin = 0;
    flag_fail_get_pin = 0;
    flag_send_ecu = 0;
    flag_send_ecu_count = 0;
}

TEST_TEAR_DOWN(reb_app)
{
    // Cleanup after tests
}

TEST(reb_app, application_init_OK)
{
    uint8_t status = 0;
    mock_can_open_return = 0;
    mock_can_ioctl_return = 2;
    status = application_init();
    TEST_ASSERT_EQUAL(SUCCESS, status);
}

TEST(reb_app, application_init_FAIL)
{
    uint8_t status = 0;
    mock_can_ioctl_return = 2;
    mock_can_open_return = 1;
    status = application_init();
    TEST_ASSERT_EQUAL(FAIL, status);
}


TEST(reb_app, cancel_reb_success)
{
    mock_can_write_return = 0;

    uint8_t result = cancel_reb();

    TEST_ASSERT_EQUAL(SUCCESS, result);
    TEST_ASSERT_EQUAL(REB_CANCELED, flag_reb_canceled);
}

TEST(reb_app, cancel_reb_fail_ipc)
{
    mock_can_write_return = 1;

    uint8_t result = cancel_reb();

    TEST_ASSERT_EQUAL(FAIL, result);
    TEST_ASSERT_EQUAL(REB_CANCELED, flag_reb_canceled);
}

TEST(reb_app, start_reb_success)
{
    flag_reb_canceled = REB_RUNNING;
    mock_can_write_return = SUCCESS;

    uint8_t result = start_reb();

    TEST_ASSERT_EQUAL(SUCCESS, result);
    TEST_ASSERT_EQUAL(REB_RUNNING, flag_reb_canceled);
}

TEST(reb_app, start_reb_fail_send_can)
{
    flag_reb_canceled = REB_RUNNING;
    mock_can_write_return = FAIL;

    uint8_t result = start_reb();

    TEST_ASSERT_EQUAL(FAIL, result);
}

TEST(reb_app, start_reb_was_canceled_before_timeout)
{
    flag_reb_canceled = REB_CANCELED;
    mock_can_write_return = SUCCESS;

    uint8_t result = start_reb();

    TEST_ASSERT_EQUAL(SUCCESS, result);
}

TEST(reb_app, test_read_console_pin_01)
{
    pthread_t read_input_th;
    pthread_create(&read_input_th, NULL, (void*)read_input, NULL);  
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    int pipefd[2];
    pipe(pipefd);

    FILE *original_stdin = stdin;
    stdin = fdopen(pipefd[0], "r");

    char input_data[] = "pin 1 1\n";
    write(pipefd[1], input_data, strlen(input_data));

    uint8_t status = 0;
    sleep(1);

    uint8_t result = read_pin_status(&status , 1);

    stdin = original_stdin;
    pthread_cancel(read_input_th);

    close(pipefd[1]);

    TEST_ASSERT_EQUAL(1, status);
    TEST_ASSERT_EQUAL(0, result);
}

TEST(reb_app, monitor_read_can_get_handle_tcu)
{

    mock_can_read_return = 1;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    TEST_ASSERT_EQUAL(1, flag_status_pin[REB_COUNTDOWN_PIN]);
}

TEST(reb_app, monitor_read_can_get_handle_tcu_fail)
{

    mock_can_read_return = 2;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    TEST_ASSERT_EQUAL(0, flag_status_pin[REB_COUNTDOWN_PIN]);
}

TEST(reb_app, monitor_read_can_check_fail)
{

    mock_can_read_return = 4;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    TEST_ASSERT_EQUAL(0, flag_status_pin[REB_COUNTDOWN_PIN]);
}

TEST(reb_app, monitor_read_can_check_REB_AUX_comunication)
{

    mock_can_read_return = 3;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    TEST_ASSERT_EQUAL(0, flag_status_pin[REB_COUNTDOWN_PIN]);
}

TEST(reb_app, countdown_reb_not_inicialize)
{

    
    pthread_t th_countdown_reb;
    pthread_create(&th_countdown_reb, NULL, (void *)countdown_reb, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(2);

    pthread_cancel(th_countdown_reb);

    TEST_ASSERT_EQUAL(0, flag_send_ecu_count);
}

TEST(reb_app, countdown_reb_inicialize)
{
    uint8_t status = 0;
    flag_send_ecu = 1;
    status = set_pin_status(1,REB_COUNTDOWN_PIN);
    TEST_ASSERT_EQUAL(0,status);
    printf("Reb countdown waiting 20 seconds\n");
    pthread_t th_countdown_reb;
    pthread_create(&th_countdown_reb, NULL, (void *)countdown_reb, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(25);

    pthread_cancel(th_countdown_reb);

    TEST_ASSERT_GREATER_THAN(0, flag_send_ecu_count);
}
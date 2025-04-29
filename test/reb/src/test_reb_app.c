/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

/**
 * @file test_reb_app.c
 * @brief Unit tests for functions in reb/app.c
 *
 * This file contains the test suite that validates the main functions of
 * Remote Engine Blocking, in app.c, inicializacion, activation and deactivation of REB and handling
 * CAN messages. It covers success and failure scenarios, as well as different status/byte values.
 *
 * Requirements covered:
 * @requir{SwHLR_F_6}
 * @requir{SwHLR_F_8}
 * @requir{SwHLR_F_10}
 * @requir{SwHLR_F_12}
 * @requir{SwHLR_F_14}
 * @requir{SwHLR_F_15}
 */

#include "app.h"
#include "ecu.h"
#include "ecu_reb.h"
#include "mcal.h"
#include "mock_can_utils.h"
#include "pins.h"
#include "unity.h"
#include "unity_fixture.h"
#include <pthread.h>
#include <string.h>
#include <unistd.h>

extern int mock_can_write_return;
extern int mock_can_open_return;
extern int mock_can_ioctl_return;
extern int flag_fail_set_pin;
extern int flag_fail_get_pin;
extern int flag_fail_get_pin_counter;
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
    flag_fail_get_pin_counter = 0;
}

TEST_TEAR_DOWN(reb_app)
{
    // Cleanup after tests
}

/**
 * @brief Tests application_init() with return status OK
 *
 * Scenario:
 *  - mcal_init() = SUCCESS
 *  - can_init() = SUCCESS
 * Expected:
 *  - Return SUCCESS (0).
 */

TEST(reb_app, application_init_OK)
{
    uint8_t status = 0;
    mock_can_open_return = 0;
    mock_can_ioctl_return = 2;
    status = application_init();
    TEST_ASSERT_EQUAL(SUCCESS, status);
}

/**
 * @brief Tests application_init() with return status FAIL
 *
 * Scenario:
 *  - mcal_init() = SUCCESS
 *  - can_init() = FAIL
 * Expected:
 *  - Return FAIL (1).
 */

TEST(reb_app, application_init_FAIL)
{
    uint8_t status = 0;
    mock_can_ioctl_return = 2;
    mock_can_open_return = 1;
    status = application_init();
    TEST_ASSERT_EQUAL_INT(FAIL, status);
}

/**
 * @brief Tests cancel_reb() function.
 *
 * Scenario:
 *  - Set REB_CANCELED to high.
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SwHLR_F_8}
 */

TEST(reb_app, cancel_reb_success)
{
    mock_can_write_return = 0;

    uint8_t result = cancel_reb();

    TEST_ASSERT_EQUAL(SUCCESS, result);
    TEST_ASSERT_EQUAL(REB_CANCELED, flag_reb_canceled);
}

/**
 * @brief Tests cancel_reb() function.
 *
 * Scenario:
 *  - CAN is unavailable when send cancel reb messsage.
 * Expected:
 *  - Return FAIL (1).
 * @requir{SwHLR_F_8}
 */

TEST(reb_app, cancel_reb_fail_ipc)
{
    mock_can_write_return = 1;

    uint8_t result = cancel_reb();

    TEST_ASSERT_EQUAL(FAIL, result);
    TEST_ASSERT_EQUAL(REB_CANCELED, flag_reb_canceled);
}


/**
 * @brief Tests cancel_reb() function.
 *
 * Scenario:
 *  - CAN is avaliable for ipc messsage.
 *  - CAN is unavailable for ecu message.
 * Expected:
 *  - Return FAIL (1).
 * @requir{SwHLR_F_8}
 */

 TEST(reb_app, cancel_reb_fail_ecu)
 {
     mock_can_write_return = 2;
 
     uint8_t result = cancel_reb();
 
     TEST_ASSERT_EQUAL(FAIL, result);
     TEST_ASSERT_EQUAL(REB_CANCELED, flag_reb_canceled);
 }

/**
 * @brief Tests start_reb() function.
 *
 * Scenario:
 *  - Set the activating REB to HIGH.
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SwHLR_F_8}
 * @requir{SwHLR_F_12}
 * @requir{SwHLR_F_14}
 * @requir{SwHLR_F_2}
 */

TEST(reb_app, start_reb_success)
{

    mock_can_write_return = SUCCESS;

    uint8_t result = start_reb();

    TEST_ASSERT_EQUAL(SUCCESS, result);
    TEST_ASSERT_EQUAL(REB_RUNNING, flag_reb_canceled);
}

/**
 * @brief Tests start_reb() function.
 *
 * Scenario:
 *  - CAN is unavailable when send start reb messsage.
 * Expected:
 *  - Return FAIL (1).
 * @requir{SwHLR_F_8}
 * @requir{SwHLR_F_12}
 * @requir{SwHLR_F_14}
 * @requir{SwHLR_F_2}
 */

TEST(reb_app, start_reb_fail_send_can)
{
    mock_can_write_return = FAIL;

    uint8_t result = start_reb();

    TEST_ASSERT_EQUAL(FAIL, result);
}

/**
 * @brief Tests read_input() function.
 *
 * Scenario:
 *  - insert input "pin 1 1".
 * Expected:
 *  - Return SUCCESS (0).
 */

TEST(reb_app, read_console)
{
    pthread_t read_input_th;
    pthread_create(&read_input_th, NULL, (void *)read_input, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);

    int pipefd[2];
    pipe(pipefd);

    char input_data[] = "pin 1 1\n";
    write(pipefd[1], input_data, strlen(input_data));

    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    sleep(1);
    uint8_t status = 0;
    uint8_t result = read_pin_status(&status, 1);

    pthread_cancel(read_input_th);

    TEST_ASSERT_EQUAL(1, status);
    TEST_ASSERT_EQUAL(0, result);
}

/**
 * @brief Tests read_input() function.
 *
 * Scenario:
 *  - Pin unavailable to change status.
 * Expected:
 *  - Return FAIL (1).
 */

TEST(reb_app, read_console_fail)
{
    flag_fail_set_pin = 1;

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

/** @brief Tests monitor_read_can() function to SUCCESS.
 *
 * Scenario:
 *  - Reb Activate Pin is set.
 * Expected:
 *  - A message CAN should be send.
 *  @requir{SwHLR_F_3}
 */
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

/** @brief Tests monitor_read_can() function to FAIL.
 *
 * Scenario:
 *  - can_read_vcan0(&frame) FAILED.
 * Expected:
 *  - Expected DTC_MONITOR_READ_CAN_FAIL) in Output
 *  @requir{SwHLR_F_3}
 */
TEST(reb_app, monitor_read_can_read_can_fail)
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

/** @brief Tests monitor_read_can() function to FAIL.
 *
 * Scenario:
 *  - can_read_vcan0(&frame) SUCCES
 *  - handle_tcu_can(frame.data) FAIL
 * Expected:
 *  - Expected Output handle_tcu_can FAIL
 *  @requir{SwHLR_F_3}
 */
TEST(reb_app, monitor_read_can_get_handle_tcu_fail)
{

    mock_can_read_return = 1;
    mock_can_write_return = 3;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    TEST_ASSERT_EQUAL(1, flag_status_pin[REB_COUNTDOWN_PIN]);
}



/** @brief Tests monitor_read_can() function to FAIL.
 *
 * Scenario:
 *  - Can is busy when check can comunication with aux.
 * Expected:
 *  - function return FAIL status.
 *  @requir{SwHLR_F_3}
 */
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

/** @brief Tests monitor_read_can(), AUX_REB CAN communication.
 *
 * Scenario:
 *  - AUX require response status from REB.
 * Expected:
 *  - REB send can message status OK.
 *  @requir{SwHLR_F_3}
 *  @requir{SwHLR_F_15}
 *  @requir{SwHLR_F_13}
 *  @requir{SwHLR_F_6}
 */
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


/** @brief Tests monitor_read_can(), AUX_REB CAN communication.
 *
 * Scenario:
 *  - when can_read_vcan0(&frame) the frame returned is correct ID, but frame.data[0] = 0xFC (Unexpected Frame).
 * Expected:
 *  - Expected DTC_MONITOR_READ_CAN_FAIL in Output.
 *  @requir{SwHLR_F_3}
 *  @requir{SwHLR_F_15}
 *  @requir{SwHLR_F_13}
 *  @requir{SwHLR_F_6}
 */
TEST(reb_app, monitor_read_can_check_WRONG_FRAME)
{

    mock_can_read_return = 5;

    pthread_t th_monitor_read_can;
    pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(1);

    pthread_cancel(th_monitor_read_can);

    TEST_ASSERT_EQUAL(0, flag_status_pin[REB_COUNTDOWN_PIN]);
}


/** @brief Tests monitor_read_can(), when can_send failed
 *
 * Scenario:
 *  - can_send_vcan0(&response) Failed
 * Expected:
 *  - Expected DTC_CAN_RESPONSE_FAIL
 *  @requir{SwHLR_F_3}
 *  @requir{SwHLR_F_15}
 *  @requir{SwHLR_F_13}
 *  @requir{SwHLR_F_6}
 */
 TEST(reb_app, monitor_read_can_CAN_SEND_FAIL)
 {
     mock_can_read_return = 3;
     mock_can_write_return = -1;
 
     pthread_t th_monitor_read_can;
     pthread_create(&th_monitor_read_can, NULL, (void *)monitor_read_can, NULL);
     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
 
     sleep(1);
     pthread_cancel(th_monitor_read_can);
 
     TEST_ASSERT_EQUAL(0, flag_status_pin[REB_COUNTDOWN_PIN]);
 }
 

/** @brief Tests countdown_reb() function to FAIL.
 *
 * Scenario:
 *  - With the activation of REB, the countdown should not start.
 * Expected:
 *  - Should not inicialize the counting.
 *  @requir{SwHLR_F_14}
 */

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

/** @brief Tests countdown_reb() function to FAIL.
 *
 * Scenario:
 *  - With the activation of REB, the countdown should not start
 *  - read_pin_status(&reb_countdown_active, REB_COUNTDOWN_PIN) FAIL
 * Expected:
 *  - Should not inicialize the counting.
 *  @requir{SwHLR_F_14}
 */

 TEST(reb_app, countdown_reb_not_inicialize_READ_PIN_FAIL)
 {
 
     pthread_t th_countdown_reb;
     flag_fail_get_pin = 1;
     pthread_create(&th_countdown_reb, NULL, (void *)countdown_reb, NULL);
     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
 
     sleep(2);
 
     pthread_cancel(th_countdown_reb);
 
     TEST_ASSERT_EQUAL(0, flag_send_ecu_count);
 }


/** @brief Tests countdown_reb() function to SUCCESS.
 *
 * Scenario:
 *  - With the activation of REB, the countdown should start.
 * Expected:
 *  - Inicialize the counting.
 *  @requir{SwHLR_F_14}
 */

TEST(reb_app, countdown_reb_inicialize)
{
    uint8_t status = 0;
    flag_send_ecu = 1;
    status = set_pin_status(1, REB_COUNTDOWN_PIN);
    TEST_ASSERT_EQUAL(0, status);
    printf("Reb countdown waiting 20 seconds\n");
    pthread_t th_countdown_reb;
    pthread_create(&th_countdown_reb, NULL, (void *)countdown_reb, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

    sleep(25);

    pthread_cancel(th_countdown_reb);

    TEST_ASSERT_GREATER_THAN(0, flag_send_ecu_count);
}


/** @brief Tests countdown_reb() function to SUCCESS.
 *
 * Scenario:
 *  - With the activation of REB, the countdown should start.
 * Expected:
 *  - Inicialize the counting.
 *  @requir{SwHLR_F_14}
 */

 TEST(reb_app, countdown_reb_inicialize_CAN_FAIL)
 {
     uint8_t status = 0;
     //flag_send_ecu = 1; 
     status = set_pin_status(1, REB_COUNTDOWN_PIN);
     flag_fail_get_pin_counter = 2;
     flag_fail_get_pin = 2;
     TEST_ASSERT_EQUAL(0, status);
     printf("Reb countdown waiting 20 seconds\n");
     pthread_t th_countdown_reb;
     pthread_create(&th_countdown_reb, NULL, (void *)countdown_reb, NULL);
     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
    
     sleep(17);
     mock_can_write_return = -1;
     flag_fail_set_pin = 2;
     sleep(8);
     pthread_cancel(th_countdown_reb);
 
     TEST_ASSERT_GREATER_THAN(-1, flag_send_ecu_count);
 }
 

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

extern int mock_can_write_return;
extern int mock_can_open_return;
extern int mock_can_ioctl_return;
extern int flag_fail_set_pin;
extern int flag_fail_get_pin;
extern int flag_cout_set_pin[10];
extern int flag_status_pin[10];

// Declaração da função read_input que já existe
extern uint8_t read_input();  

// Wrapper que chama read_input() e retorna NULL para compatibilidade com pthread_create
void* read_input_wrapper(void* arg)
{
    read_input();  // Chama sua função original
    return NULL;   // Retorna NULL para atender à assinatura esperada por pthread_create
}


TEST_GROUP(reb_app);

TEST_SETUP(reb_app)
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


TEST(reb_app, test_read_console_pin_01)
{
    pthread_t read_input_th;
    pthread_create(&read_input_th, NULL, read_input_wrapper, NULL);  
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
/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "ecu_reb.h"
#include "unity.h"
#include "unity_fixture.h"
#include "app.h"
#include "pthread.h"
#include "string.h"

extern int mock_can_write_return;

TEST_GROUP(reb_app);

// sometimes you may want to get at local data in a module.
// for example: If you plan to pass by reference, this could be useful
// however, it should often be avoided

TEST_SETUP(reb_app)
{
    // This is run before EACH TEST
}

TEST_TEAR_DOWN(reb_app) {}

TEST(reb_app, test_read_console_pin_01)
{
    pthread_t read_input_th;
    pthread_create(&read_input_th, NULL, read_input, NULL);
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

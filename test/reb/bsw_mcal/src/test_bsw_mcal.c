/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "mcal.h"
#include "unity.h"
#include "unity_fixture.h"
#include "mock_can_utils.h"

TEST_GROUP(mcal_others);

extern int Counter;

TEST_SETUP(mcal_others) {}

TEST_TEAR_DOWN(mcal_others) {}

TEST(mcal_others, get_status_pin_0)
{
    // All of these should pass
    uint8_t status = 0;
    read_pin_status(&status, 0);

    TEST_ASSERT_EQUAL_UINT8(0, status);
}

TEST(mcal_others, mcal_init_SUCCESS)
{
    uint8_t result = mcal_init();
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

TEST(mcal_others, go_sleep_1)
{
    uint8_t result = 0;
    go_sleep(1);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

TEST(mcal_others, show_error_TEXT)
{
    uint8_t result = 0;
    show_error("show error test");
    TEST_ASSERT_EQUAL_UINT8(0, result);
}


void *my_func(void *arg) {
    uint8_t value = 0;
    (void)value; // Avoid unused variable warning
    return NULL;
}

TEST(mcal_others, new_thread_SUCCESS)
{
    pthread_t result = new_thread(my_func);
    
    // Verify that the thread ID is not zero (which means the thread was successfully created)
    TEST_ASSERT_NOT_EQUAL(0, result);
    
    // Join the thread to ensure cleanup
    pthread_join(result, NULL);
}





TEST_GROUP(mcal_can);

extern int Counter;

TEST_SETUP(mcal_can)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
}

TEST_TEAR_DOWN(mcal_can) 
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
}


//tets can interface
TEST(mcal_can, can_interface_status_UP)
{
    set_mock_return_values(2, 0, 0, 0, 0, 0);
    int can_socket = 1;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(0, result);
}
TEST(mcal_can, can_interface_status_DOWN)
{
    set_mock_return_values(1, 0, 0, 0, 0, 0);
    int can_socket = 2;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

TEST(mcal_can, can_interface_status_DOES_NOT_EXIST)
{
    set_mock_return_values(-1, 0, 0, 0, 0, 0);
    int can_socket = -1;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

// can bind socket
TEST(mcal_can, can_bind_socket_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    int can_socket = 0;
    uint8_t result = can_bind_socket(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(0, result);
}
TEST(mcal_can, can_bind_socket_FAIL)
{
    set_mock_return_values(0, -1, 0, 0, 0, 0);
    int can_socket = 2;
    uint8_t result = can_bind_socket(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

//can open
TEST(mcal_can, can_socket_open_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    int can_socket = 0;
    uint8_t result = can_socket_open(&can_socket);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}
TEST(mcal_can, can_socket_open_FAIL)
{
    set_mock_return_values(0, 0, 0, 0, -1, 0);
    int can_socket = 0;
    uint8_t result = can_socket_open(&can_socket);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}



//can close
TEST(mcal_can, can_socket_close_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    int can_socket = 0;
    uint8_t result = can_socket_close(&can_socket);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}
TEST(mcal_can, can_socket_close_FAIL)
{
    set_mock_return_values(0, 0, 0, 0, 0, -1);
    int can_socket = -1;
    uint8_t result = can_socket_close(&can_socket);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}


TEST(mcal_can, can_close_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    uint8_t result = can_close();
    TEST_ASSERT_EQUAL_UINT8(0, result);
}


TEST(mcal_can, can_send_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    int can_socket = 0;
    struct can_frame frame;
    uint8_t result = can_send(&can_socket,&frame);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

TEST(mcal_can, can_send_FAIL)
{
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    int can_socket = 0;
    struct can_frame frame;
    uint8_t result = can_send(&can_socket,&frame);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

TEST(mcal_can, can_send_vcan0_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    struct can_frame frame;
    uint8_t result = can_send_vcan0(&frame);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}




TEST(mcal_can, can_read_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    int can_socket = 0;
    struct can_frame frame;
    uint8_t result = can_read(&can_socket,&frame);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

TEST(mcal_can, can_read_FAIL)
{
    set_mock_return_values(0, 0, 0, -1, 0, 0);
    int can_socket = 0;
    struct can_frame frame;
    uint8_t result = can_read(&can_socket,&frame);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

TEST(mcal_can, can_read_vcan0_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    struct can_frame frame;
    uint8_t result = can_read_vcan0(&frame);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}


//can start 
TEST(mcal_can, can_start_SUCCESS)
{
    set_mock_return_values(2, 0, 0, 0, 0, 0);
    int can_socket = 0;
    uint8_t result = can_start(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

TEST(mcal_can, can_start_SOCKET_FAIL)
{
    set_mock_return_values(-1, 0, 0, 0, 0, 0);
    int can_socket = -1;
    uint8_t result = can_start(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

TEST(mcal_can, can_start_INTERFACE_FAIL)
{
    int can_socket = -1;
    uint8_t result = can_start(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

TEST(mcal_can, can_start_BIND_FAIL)
{
    set_mock_return_values(0, -1, 0, 0, 0, 0);
    int can_socket = -1;
    uint8_t result = can_start(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

TEST(mcal_can, can_init_SUCCESS)
{
    int can_socket = -1;
    uint8_t result = can_init();
    TEST_ASSERT_EQUAL_UINT8(1, result);
}
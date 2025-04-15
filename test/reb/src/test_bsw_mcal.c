/**
 * @file test_bsw_mcal.c
 * @brief Unit tests for functions in mcal.c
 *
 * This file contains the test suite that validates the main functions of
 * Remote Engine Blocking, in mcal, focusing on handling CAN socket, interface and communication. It
 * also handles digital IO. It covers success and failure scenarios.
 *
 * Requirements covered:
 * - SysHLR_9
 */

#include "mcal.h"
#include "mock_can_utils.h"
#include "unity.h"
#include "unity_fixture.h"
#include <stdio.h>

extern int Counter;
extern int flag_fail_set_pin;

TEST_GROUP(mcal_others);

TEST_SETUP(mcal_others) {}

TEST_TEAR_DOWN(mcal_others) {}

/**
 * @brief Tests set_pin_status() when pin number is invalid.
 *
 * Scenario:
 *  - The pin number is 10, which is higher than the avaliable IO.
 * Expected:
 *  - Return FAILURE (1).
 */
TEST(mcal_others, set_pin_status_FAIL_PIN_NUMBER)
{
    flag_fail_set_pin = 0;
    uint8_t pin = 10, status = 0;
    uint8_t result = set_pin_status(status, pin);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests set_pin_status() when pin number is valid.
 *
 * Scenario:
 *  - The pin number is 1, which is lower than the avaliable IO.
 * Expected:
 *  - Return SUCCESS (0).
 */
TEST(mcal_others, set_pin_status_SUCCESS)
{
    uint8_t pin = 1, status = 0;
    uint8_t result = set_pin_status(status, pin);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests read_pin_status() when pin number is invalid.
 *
 * Scenario:
 *  - The pin number is 10, which is higher than the avaliable IO.
 * Expected:
 *  - Return FAILURE (1).
 */
TEST(mcal_others, read_pin_status_FAIL_PIN_NUMBER)
{
    uint8_t pin = 10, status = 0;
    uint8_t result = read_pin_status(&status, pin);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests read_pin_status() when pin number is valid.
 *
 * Scenario:
 *  - The pin number is 1, which is lower than the avaliable IO.
 * Expected:
 *  - Return SUCCESS (0).
 */
TEST(mcal_others, read_pin_status_SUCCESS)
{
    uint8_t pin = 1, status = 0;
    uint8_t result = read_pin_status(&status, pin);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests read_pint_status() when status is 0.
 *
 * Scenario:
 *  - A valid pin (5) and status (0) are provided.
 *  - The function reads the correct values from the simulated input.
 * Expected:
 *  - Return SUCCESS (0).
 *  - Pin value should be 5, and status should be 0.
 */
TEST(mcal_others, read_pint_status_SUCCESS_STATUS_0)
{
    uint8_t pin = 0, status = 0;
    FILE *mock_stdin = fmemopen("pin 5 0\n", 8, "r");
    stdin = mock_stdin;
    uint8_t result = read_pint_status(&pin, &status);
    fclose(mock_stdin);

    TEST_ASSERT_EQUAL_UINT8(0, result);
    TEST_ASSERT_EQUAL_UINT8(5, pin);
    TEST_ASSERT_EQUAL_UINT8(0, status);
}

/**
 * @brief Tests read_pint_status() when status is 1.
 *
 * Scenario:
 *  - A valid pin (5) and status (1) are provided.
 *  - The function reads the correct values from the simulated input.
 * Expected:
 *  - Return SUCCESS (0).
 *  - Pin value should be 5, and status should be 1.
 */
TEST(mcal_others, read_pint_status_SUCCESS_STATUS_1)
{
    uint8_t pin = 0, status = 0;
    FILE *mock_stdin = fmemopen("pin 5 1\n", 8, "r");
    FILE *original_stdin = stdin;
    stdin = mock_stdin;
    uint8_t result = read_pint_status(&pin, &status);
    stdin = original_stdin;
    fclose(mock_stdin);

    TEST_ASSERT_EQUAL_UINT8(0, result);
    TEST_ASSERT_EQUAL_UINT8(5, pin);
    TEST_ASSERT_EQUAL_UINT8(1, status);
}

/**
 * @brief Tests read_pint_status() when getline fails due to stdin being closed.
 *
 * Scenario:
 *  - Simulates a failure in getline by closing stdin.
 *  - The function should handle the failure gracefully.
 * Expected:
 *  - Return FAILURE (1).
 */
TEST(mcal_others, read_pint_status_FAIL_GETLINE)
{
    uint8_t pin = 0, status = 0;
    fclose(stdin);
    uint8_t result = read_pint_status(&pin, &status);

    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests read_pint_status() when input line has the wrong prefix.
 *
 * Scenario:
 *  - Simulates an invalid input where the line does not start with the expected "pin" prefix.
 * Expected:
 *  - Return FAILURE (1).
 */
TEST(mcal_others, read_pint_status_FAIL_WRONG_PREFIX)
{
    uint8_t pin = 0, status = 0;
    FILE *mock_stdin = fmemopen("wrong 5 1\n", 10, "r");
    stdin = mock_stdin;
    uint8_t result = read_pint_status(&pin, &status);
    fclose(mock_stdin);

    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests read_pint_status() when the input contains an invalid status.
 *
 * Scenario:
 *  - Simulates an input where the status is an invalid value (not 0 or 1).
 * Expected:
 *  - Return FAILURE (1).
 */
TEST(mcal_others, read_pint_status_FAIL_INVALID_STATUS)
{
    uint8_t pin = 0, status = 0;
    FILE *mock_stdin = fmemopen("pin 5 99\n", 10, "r");
    stdin = mock_stdin;
    uint8_t result = read_pint_status(&pin, &status);
    fclose(mock_stdin);

    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests read_pint_status() when sscanf fails to parse input.
 *
 * Scenario:
 *  - Simulates an input where sscanf fails to parse the pin and status due to invalid characters.
 * Expected:
 *  - Return FAILURE (1).
 */
TEST(mcal_others, read_pint_status_FAIL_SSCANF)
{
    uint8_t pin = 0, status = 0;

    // Simula entrada inválida para sscanf
    FILE *mock_stdin = fmemopen("pin abc xyz\n", 13, "r");
    stdin = mock_stdin;

    uint8_t result = read_pint_status(&pin, &status);

    fclose(mock_stdin);

    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests read_pint_status() for edge case with missing status value.
 *
 * Scenario:
 *  - Simulates an input where the status value is missing (extra space).
 * Expected:
 *  - Return FAILURE (1).
 */
TEST(mcal_others, read_pint_status_FAIL_EDGE_CASE)
{
    uint8_t pin = 0, status = 0;
    FILE *mock_stdin = fmemopen("pin 5 \n", 8, "r");
    FILE *original_stdin = stdin;
    stdin = mock_stdin;
    uint8_t result = read_pint_status(&pin, &status);
    stdin = original_stdin;
    fclose(mock_stdin);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests mcal_init() when all pin structures are initialized successfully.
 *
 * Scenario:
 *  - Iterates through all I/O pins and sets default values.
 * Expected:
 *  - All pins initialized with status 0 and corresponding pin numbers.
 *  - Return SUCCESS (0).
 */
TEST(mcal_others, mcal_init_SUCCESS)
{
    uint8_t result = mcal_init();
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests go_sleep() with 1 second delay.
 *
 * Scenario:
 *  - go_sleep() is called with a delay of 1 second.
 * Expected:
 *  - The function completes without altering the result variable.
 */
TEST(mcal_others, go_sleep_1)
{
    uint8_t result = 0;
    go_sleep(1);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests show_error() when displaying an error message.
 *
 * Scenario:
 *  - A text message is passed to show_error().
 * Expected:
 *  - The function prints the message and does not modify the input result.
 * @requir{SwHLR_F_16}
 */
TEST(mcal_others, show_error_TEXT)
{
    uint8_t result = 0;
    show_error("show error test");
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

void *my_func(void *arg)
{
    uint8_t value = 0;
    (void)value; // Avoid unused variable warning
    return NULL;
}

/**
 * @brief Tests new_thread() when thread creation is successful.
 *
 * Scenario:
 *  - Thread function is valid and thread creation succeeds.
 * Expected:
 *  - A non-zero thread ID is returned and pthread_join completes without error.
 */
TEST(mcal_others, new_thread_SUCCESS)
{
    pthread_t result = new_thread(my_func);
    TEST_ASSERT_NOT_EQUAL(0, result);
    pthread_join(result, NULL);
}

TEST_GROUP(mcal_can);

extern int Counter;

/**
 * @brief Sets up mock return values before each test in the mcal_can suite.
 *
 * Scenario:
 *  - Before each test in the mcal_can suite, the mock return values are set to default (0).
 * Expected:
 */
TEST_SETUP(mcal_can) { set_mock_return_values(0, 0, 0, 0, 0, 0); }

/**
 * @brief Resets mock return values after each test in the mcal_can suite.
 *
 * Expected:
 *  - Mock return values are set to default (0) to ensure no side effects between tests.
 * @requir{SysHLR_9}
 */
TEST_TEAR_DOWN(mcal_can) { set_mock_return_values(0, 0, 0, 0, 0, 0); }

/**
 * @brief Tests can_interface_status() with INTERFACE_UP.
 *
 * Scenario:
 *  - CAN interface is UP
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_interface_status_UP)
{
    set_mock_return_values(2, 0, 0, 0, 0, 0);
    int can_socket = 1;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests can_interface_status() with INTERFACE_DOWN.
 *
 * Scenario:
 *  - CAN interface is DOWN
 * Expected:
 *  - Return FAILURE (1).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_interface_status_DOWN)
{
    set_mock_return_values(1, 0, 0, 0, 0, 0);
    int can_socket = 2;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests can_interface_status() when INTERFACE DOES NOT EXIST.
 *
 * Scenario:
 *  - CAN interface is does not exist.
 * Expected:
 *  - Return FAILURE (1).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_interface_status_DOES_NOT_EXIST)
{
    set_mock_return_values(-1, 0, 0, 0, 0, 0);
    int can_socket = -1;
    uint8_t result = can_interface_status(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests can_bind_socket() when can_bind succeeds.
 *
 * Scenario:
 *  - Mocked can_bind is set to return 0 (success).
 *  - All other mocked functions also return success.
 * Expected:
 *  - can_bind_socket() returns SUCCESS (0).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_bind_socket_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    int can_socket = 0;
    uint8_t result = can_bind_socket(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests can_bind_socket() when can_bind fails.
 *
 * Scenario:
 *  - Mocked can_bind is set to return -1 (failure).
 * Expected:
 *  - can_bind_socket() returns FAIL (1) due to bind failure.
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_bind_socket_FAIL)
{
    set_mock_return_values(0, -1, 0, 0, 0, 0);
    int can_socket = 2;
    uint8_t result = can_bind_socket(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests can_socket_open() when socket_create succeeds.
 *
 * Scenario:
 *  - Mocked socket_create is set to return a valid socket (1).
 * Expected:
 *  - can_socket_open() returns SUCCESS (0).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_socket_open_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    int can_socket = 0;
    uint8_t result = can_socket_open(&can_socket);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests can_socket_open() when socket_create fails.
 *
 * Scenario:
 *  - Mocked socket_create is set to return -1 (failure).
 * Expected:
 *  - can_socket_open() returns FAIL (1).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_socket_open_FAIL)
{
    set_mock_return_values(0, 0, 0, 0, -1, 0);
    int can_socket = 0;
    uint8_t result = can_socket_open(&can_socket);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests can_socket_close() when socket_close succeeds.
 *
 * Scenario:
 *  - Mocked socket_close is set to return success (0).
 * Expected:
 *  - can_socket_close() returns SUCCESS (0).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_socket_close_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    int can_socket = 0;
    uint8_t result = can_socket_close(&can_socket);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests can_socket_close() when socket_close fails.
 *
 * Scenario:
 *  - Mocked socket_close is set to return -1 (failure).
 * Expected:
 *  - can_socket_close() returns FAIL (1).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_socket_close_FAIL)
{
    set_mock_return_values(0, 0, 0, 0, 0, -1);
    int can_socket = -1;
    uint8_t result = can_socket_close(&can_socket);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests can_close() when can_socket_close is successful.
 *
 * Scenario:
 *  - Mocked can_socket_close is set to return success (0).
 * Expected:
 *  - can_close() returns SUCCESS (0).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_close_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    uint8_t result = can_close();
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests can_send() when can_write succeeds.
 *
 * Scenario:
 *  - Mocked can_write is set to return success (0).
 * Expected:
 *  - can_send() returns SUCCESS (0).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_send_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    int can_socket = 0;
    struct can_frame frame;
    uint8_t result = can_send(&can_socket, &frame);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests can_send() when can_write fails.
 *
 * Scenario:
 *  - Mocked can_write is set to return -1 (failure).
 * Expected:
 *  - can_send() returns FAIL (1).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_send_FAIL)
{
    set_mock_return_values(0, 0, -1, 0, 0, 0);
    int can_socket = 0;
    struct can_frame frame;
    uint8_t result = can_send(&can_socket, &frame);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests can_send_vcan0() when can_send is successful.
 *
 * Scenario:
 *  - Mocked can_send is set to return success (0).
 * Expected:
 *  - can_send_vcan0() returns SUCCESS (0).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_send_vcan0_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    struct can_frame frame;
    uint8_t result = can_send_vcan0(&frame);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests can_read() when can_read_socket succeeds.
 *
 * Scenario:
 *  - Mocked can_read_socket is set to return a positive value (indicating data was read
 * successfully). Expected:
 *  - can_read() returns SUCCESS (0).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_read_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    int can_socket = 0;
    struct can_frame frame;
    uint8_t result = can_read(&can_socket, &frame);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests can_read() when can_read_socket fails.
 *
 * Scenario:
 *  - Mocked can_read_socket is set to return -1 (failure).
 * Expected:
 *  - can_read() returns FAIL (1).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_read_FAIL)
{
    set_mock_return_values(0, 0, 0, -1, 0, 0);
    int can_socket = 0;
    struct can_frame frame;
    uint8_t result = can_read(&can_socket, &frame);
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests can_read_vcan0() when can_read_socket succeeds.
 *
 * Scenario:
 *  - Mocked can_read_socket is set to return a positive value (indicating data was read
 * successfully). Expected:
 *  - can_read_vcan0() returns SUCCESS (0).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_read_vcan0_SUCCESS)
{
    set_mock_return_values(0, 0, 0, 0, 0, 0);
    struct can_frame frame;
    uint8_t result = can_read_vcan0(&frame);
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests can_start() when all operations are successful.
 *
 * Scenario:
 *  - can_socket_open, can_interface_status, and can_bind_socket all return successful values.
 * Expected:
 *  - Return SUCCESS (0).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_start_SUCCESS)
{
    set_mock_return_values(2, 0, 0, 0, 0, 0);
    int can_socket = 0;
    uint8_t result = can_start(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(0, result);
}

/**
 * @brief Tests can_start() when socket creation fails.
 *
 * Scenario:
 *  - can_socket_open returns 1, simulating a failure in socket creation.
 * Expected:
 *  - Return FAILURE (1).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_start_SOCKET_FAIL)
{
    set_mock_return_values(2, 0, 0, 0, -1, 0);
    int can_socket = -1;
    uint8_t result = can_start(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests can_start() when the interface status check fails.
 *
 * Scenario:
 *  - can_interface_status returns 1, simulating a failure in the interface status check.
 * Expected:
 *  - Return FAILURE (1).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_start_INTERFACE_FAIL)
{
    int can_socket = -1;
    set_mock_return_values(1, 0, 0, 0, 0, 0);
    uint8_t result = can_start(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests can_start() when binding the socket fails.
 *
 * Scenario:
 *  - can_bind_socket returns 1, simulating a failure in binding the socket.
 * Expected:
 *  - Return FAILURE (1).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_start_BIND_FAIL)
{
    set_mock_return_values(2, -1, 0, 0, 0, 0);
    int can_socket = -1;
    uint8_t result = can_start(&can_socket, "vcan0");
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

/**
 * @brief Tests can_init() when initialization is successful.
 *
 * Scenario:
 *  - can_socket is successfully initialized.
 * Expected:
 *  - Return SUCCESS (1).
 * @requir{SysHLR_9}
 */
TEST(mcal_can, can_init_SUCCESS)
{
    int can_socket = -1;
    uint8_t result = can_init();
    TEST_ASSERT_EQUAL_UINT8(1, result);
}

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
    pthread_create(&read_input_th, NULL, read_input, NULL); // Note que é necessário fornecer uma função wrapper!
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    int pipefd[2]; // pipefd[0] é para leitura, pipefd[1] é para escrita
    pipe(pipefd);

    // Redireciona stdin para o lado de leitura do pipe
    FILE *original_stdin = stdin;
    stdin = fdopen(pipefd[0], "r");

    // Envia um comando de input para o pipe (lado de escrita)
    char input_data[] = "pin 1 1\n";
    write(pipefd[1], input_data, strlen(input_data));

    uint8_t pin, status;
    sleep(1);

    // Primeira leitura: Deve funcionar
    uint8_t result = read_pin_status(&pin, &status);
    TEST_ASSERT_EQUAL_UINT(1, result);
    TEST_ASSERT_EQUAL_UINT(1, status);
    TEST_ASSERT_EQUAL_UINT(1, pin);

    // Segunda leitura: Deve bloquear (a menos que tenha algo escrito no pipe)
    // Para testar o bloqueio, você pode rodar essa linha isoladamente e ver que trava.
    // result = read_pin_status(&pin, &status);

    // Fecha o lado de escrita para simular fim de entrada
    
    // Restaura o stdin original
    stdin = original_stdin;
    pthread_cancel(read_input_th);
    write(pipefd[1], input_data, strlen(input_data));
    close(pipefd[1]);

    
    // char input_data[] = "pin 1 1\n";
    // FILE *mock_stdin = fmemopen(input_data, strlen(input_data), "r");
    
    // FILE *original_stdin = stdin;
    // stdin = mock_stdin;
    
    
    // uint8_t status;
    // read_pin_status(&status, 1);

    // stdin = original_stdin;
    // fclose(mock_stdin);

    // TEST_ASSERT_EQUAL_UINT(1, status);
}

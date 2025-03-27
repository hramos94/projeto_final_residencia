#include "mcal.h"
#include "unity.h"

// Função de configuração chamada antes de cada teste
void setUp(void)
{
    // Código de inicialização antes de cada teste
}

// Função de limpeza chamada depois de cada teste
void tearDown(void)
{
    // Código de limpeza depois de cada teste
}

void test_get_pin_status(void)
{
    uint8_t status = 0;
    read_pin_status(&status, 0);

    TEST_ASSERT_EQUAL_INT16(1, status);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_get_pin_status);
    return UNITY_END();
}

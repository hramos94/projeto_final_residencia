/* =========================================================================
    Unity - A Test Framework for C
    ThrowTheSwitch.org
    Copyright (c) 2007-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "unity_fixture.h"
#include <ecu_aux.h>

TEST_GROUP(ecu_aux);

// sometimes you may want to get at local data in a module.
// for example: If you plan to pass by reference, this could be useful
// however, it should often be avoided
extern int Counter;

TEST_SETUP(ecu_aux)
{
    // This is run before EACH TEST
}

TEST_TEAR_DOWN(ecu_aux) {}

TEST(ecu_aux, tempTestQualquer)
{
    // qualquer teste;
    TEST_ASSERT_EQUAL_INT16(0, 0);
}

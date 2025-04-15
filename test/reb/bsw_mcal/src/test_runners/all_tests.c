/* =========================================================================
    unity - a test framework for c
    throwtheswitch.org
    copyright (c) 2007-25 mike karlesky, mark vandervoord, & greg williams
    spdx-license-identifier: mit
========================================================================= */

#include "unity_fixture.h"

static void RunAllTests(void)
{
    RUN_TEST_GROUP(mcal_can);
    RUN_TEST_GROUP(mcal_others);
}

int main(int argc, const char *argv[]) { return UnityMain(argc, argv, RunAllTests); }

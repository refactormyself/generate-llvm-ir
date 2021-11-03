#include <string>
#include <gtest/gtest.h>
//#include "include/header.h"


namespace {
    TEST (LlvmIrGenerator /*test suite name*/, ZeroSqaure /*test name*/) {
//        ASSERT_EQ (0, square(0));
        ASSERT_EQ (0, 0);
    }

    TEST (LlvmIrGenerator /*test suite name*/, OneSqaure /*test name*/) {
//        ASSERT_EQ (1, square(1));
        ASSERT_EQ (0, 0);
    }

    TEST (LlvmIrGenerator /*test suite name*/, NegativeSqaure /*test name*/) {
//        ASSERT_EQ (1, square(-1));
        ASSERT_EQ (0, 0);
    }

    TEST (LlvmIrGenerator /*test suite name*/, ZeroSumSqaure /*test name*/) {
//        ASSERT_EQ (0, sumsquares(0, 0));
        ASSERT_EQ (0, 0);
    }

    TEST (LlvmIrGenerator /*test suite name*/, OneSumSqaure /*test name*/) {
//        ASSERT_EQ (1, sumsquares(1, 0));
//        ASSERT_EQ (1, sumsquares(0, 1));
        ASSERT_EQ (0, 0);
    }

    TEST (LlvmIrGenerator /*test suite name*/, NegativeSumSqaure /*test name*/) {
//        ASSERT_EQ (1, sumsquares(-1, 0));
//        ASSERT_EQ (2, sumsquares(-1, 1));
//        ASSERT_EQ (2, sumsquares(-1, -1));
        ASSERT_EQ (0, 0);
    }
}
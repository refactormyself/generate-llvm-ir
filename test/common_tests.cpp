#include <string>
#include <gtest/gtest.h>


namespace {
    TEST (Sample_1 /*test suite name*/, SimpleTestShouldPass /*test name*/) {
    std::string inFileName;
    std::string outFileName = "../out_file";

    EXPECT_TRUE(inFileName.empty());
    EXPECT_FALSE(outFileName.empty());

    ASSERT_EQ (true, inFileName.empty());  // drop to repl mode
}

}
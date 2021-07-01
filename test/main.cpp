#include <gtest/gtest.h>

// Add custom main so we test independently of the cli interface. We expect our library to be well tested :)
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
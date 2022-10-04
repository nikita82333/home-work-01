#include "lib.h"

#include "gtest/gtest.h"

TEST(google_test, test_version)
{
    EXPECT_GT(version(), 1);
}
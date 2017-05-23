#include <limits.h>
#include "gtest/gtest.h"
#include "drive_lib.h"

int result=0;

TEST(recordTest, simple){
  result=write_file();
  EXPECT_EQ(1, result);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
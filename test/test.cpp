#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"
#include "../pixelmap.h"


// IndependentMethod is a test case - here, we have 2 tests for this 1 test case
TEST(IndependentMethod, ResetsToZero) {
  int i = 3;
  independentMethod(i);
  EXPECT_EQ(0, i);

  i = 12;
  independentMethod(i);
  EXPECT_EQ(0,i);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

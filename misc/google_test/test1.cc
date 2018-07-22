
#include "tmp.h"
#include "gtest/gtest.h"

TEST(Test1, test11)
{
  ASSERT_EQ(ret1(), 1);
  ASSERT_EQ(ret2(), 2);
  ASSERT_EQ(ret3(), 3);
  ASSERT_EQ(ret4(), 4);
}

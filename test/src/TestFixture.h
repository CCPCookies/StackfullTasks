#include <gtest/gtest.h>

struct TestFixture : public ::testing::Test
{
	void SetUp();

	void TearDown();

	inline static int s_testInt = 0;

};
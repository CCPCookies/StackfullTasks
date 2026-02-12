
#include "TestFixture.h"
#include <Task.h>
#include <Tasklet.h>

struct TaskletppLibraryTest : public TestFixture
{
};

// Test functions
void TestTaskletYeildFunction(Tasklet* tasklet)
{
    TestFixture::s_testInt = 1;

    tasklet->Yield();

    TestFixture::s_testInt = 2;

    tasklet->Yield();

    TestFixture::s_testInt = 3;
}

void TestTaskletYeildFunctionWithBase(Tasklet* tasklet)
{
    TestFixture::s_testInt = -1;

    tasklet->Yield();

    TestTaskletYeildFunction(tasklet);
}

void TestTaskletKill(Tasklet* tasklet)
{
    TestFixture::s_testInt = 1;
    
    if (tasklet->Yield())
    {
        TestFixture::s_testInt = 2;
    }
    else
    {
        TestFixture::s_testInt = 3;
    }

}

void TestTaskletOneFunctionDeep()
{
    Tasklet tasklet(TestTaskletYeildFunction);

    EXPECT_TRUE(tasklet.Run());
    EXPECT_TRUE(tasklet.Run());
    EXPECT_TRUE(tasklet.Run());

}

void TestTaskletRun(Tasklet& tasklet)
{
    EXPECT_TRUE(tasklet.Run());
}

// ---Tasklet Tests---
TEST_F(TaskletppLibraryTest, TestSingleTaskletYeild)
{
    Tasklet tasklet(TestTaskletYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 2);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 3);
}

TEST_F(TaskletppLibraryTest, TestSingleCalledOneFunctionDeepTasklet)
{
    TestTaskletOneFunctionDeep();

    EXPECT_EQ(s_testInt, 3);
}

TEST_F(TaskletppLibraryTest, TestYieldThenRunFromDeeperLayer)
{
    Tasklet tasklet(TestTaskletYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    TestTaskletRun(tasklet);

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(TaskletppLibraryTest, TestSingleTaskletWithBase)
{
    Tasklet tasklet(TestTaskletYeildFunctionWithBase);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, -1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 2);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 3);
}

TEST_F(TaskletppLibraryTest, TestSingleTaskletKill)
{
    Tasklet tasklet(TestTaskletKill);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Kill());

    EXPECT_EQ(s_testInt, 3);

}

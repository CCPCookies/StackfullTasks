
#include "TestFixture.h"
#include <Task.h>
#include <Tasklet.h>

struct TaskletTest : public TestFixture
{
};

// Test functions
void TestTaskletEmpyFunction(Tasklet* tasklet)
{
    TestFixture::s_testInt = 1;
}

void TestTaskletYeildFunction(Tasklet* tasklet)
{
    TestFixture::s_testInt = 1;

    tasklet->Yield();

    TestFixture::s_testInt = 2;
}

void TestTaskletMultiYeildFunction(Tasklet* tasklet)
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

}

void TestTaskletRun(Tasklet& tasklet)
{
    EXPECT_TRUE(tasklet.Run());
}

// ---Tasklet Tests---
TEST_F(TaskletTest, TestSimpleTaskletSwitch)
{
    Tasklet tasklet(TestTaskletEmpyFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), TaskletState::FINISHED);

    EXPECT_EQ(s_testInt, 1);
}

TEST_F(TaskletTest, TestSingleTaskletYeild)
{
    Tasklet tasklet(TestTaskletYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), TaskletState::SUSPENDED);

    // Should have returned before setting s_testInt to 2
    EXPECT_EQ(s_testInt, 1);
}

TEST_F(TaskletTest, TestSingleTaskletYeildAndResume)
{
    Tasklet tasklet(TestTaskletYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), TaskletState::SUSPENDED);

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), TaskletState::FINISHED);

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(TaskletTest, TestSingleTaskletMultipleYeilds)
{
    Tasklet tasklet(TestTaskletMultiYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 2);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 3);
}

TEST_F(TaskletTest, TestSingleTaskletYeildAndKill)
{
    Tasklet tasklet(TestTaskletKill);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), TaskletState::SUSPENDED);

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Kill());

    EXPECT_EQ(tasklet.GetState(), TaskletState::FINISHED);

    EXPECT_EQ(s_testInt, 3);
}

TEST_F(TaskletTest, TestSingleCalledOneFunctionDeepTasklet)
{
    TestTaskletOneFunctionDeep();

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(TaskletTest, TestYieldThenRunFromDeeperLayer)
{
    Tasklet tasklet(TestTaskletYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    TestTaskletRun(tasklet);

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(TaskletTest, TestSingleTaskletWithBase)
{
    Tasklet tasklet(TestTaskletYeildFunctionWithBase);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, -1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 2);

    EXPECT_EQ(tasklet.GetState(), TaskletState::FINISHED);

}

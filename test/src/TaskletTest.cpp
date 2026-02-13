
#include "TestFixture.h"
#include <Task.h>
#include <Tasklet.h>

struct TaskletTest : public TestFixture
{
};

// Test functions
void TestTaskletEmpyFunction(StackfullTasks::Tasklet* tasklet)
{
    TestFixture::s_testInt = 1;
}

void TestTaskletYeildFunction(StackfullTasks::Tasklet* tasklet)
{
    TestFixture::s_testInt = 1;

    tasklet->Yield();

    TestFixture::s_testInt = 2;
}

void TestTaskletMultiYeildFunction(StackfullTasks::Tasklet* tasklet)
{
    TestFixture::s_testInt = 1;

    tasklet->Yield();

    TestFixture::s_testInt = 2;

    tasklet->Yield();

    TestFixture::s_testInt = 3;
}

void TestTaskletYeildFunctionWithBase(StackfullTasks::Tasklet* tasklet)
{
    TestFixture::s_testInt = -1;

    tasklet->Yield();

    TestTaskletYeildFunction(tasklet);
}

void TestTaskletKill(StackfullTasks::Tasklet* tasklet)
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
    StackfullTasks::Tasklet tasklet(TestTaskletYeildFunction);

    EXPECT_TRUE(tasklet.Run());
    EXPECT_TRUE(tasklet.Run());

}

void TestTaskletRun(StackfullTasks::Tasklet& tasklet)
{
    EXPECT_TRUE(tasklet.Run());
}

void TestCreateAndRunTasklet(StackfullTasks::Tasklet* activeTasklet)
{
    TestFixture::s_testInt = 1;

    activeTasklet->Yield();

    StackfullTasks::Tasklet innerTasklet(TestTaskletYeildFunction);

    EXPECT_TRUE(innerTasklet.Run());

    EXPECT_EQ(TestFixture::s_testInt, 1);

    EXPECT_TRUE(innerTasklet.Run());

    EXPECT_EQ(TestFixture::s_testInt, 2);

    TestFixture::s_testInt = 3;
}

// ---Tasklet Tests---
TEST_F(TaskletTest, TestSimpleTaskletSwitch)
{
    StackfullTasks::Tasklet tasklet(TestTaskletEmpyFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::FINISHED);

    EXPECT_EQ(s_testInt, 1);
}

TEST_F(TaskletTest, TestSingleTaskletYeild)
{
    StackfullTasks::Tasklet tasklet(TestTaskletYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::SUSPENDED);

    // Should have returned before setting s_testInt to 2
    EXPECT_EQ(s_testInt, 1);
}

TEST_F(TaskletTest, TestSingleTaskletYeildAndResume)
{
    StackfullTasks::Tasklet tasklet(TestTaskletYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::SUSPENDED);

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::FINISHED);

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(TaskletTest, TestSingleTaskletMultipleYeilds)
{
    StackfullTasks::Tasklet tasklet(TestTaskletMultiYeildFunction);

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
    StackfullTasks::Tasklet tasklet(TestTaskletKill);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::SUSPENDED);

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Kill());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::FINISHED);

    EXPECT_EQ(s_testInt, 3);
}

TEST_F(TaskletTest, TestSingleCalledOneFunctionDeepTasklet)
{
    TestTaskletOneFunctionDeep();

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(TaskletTest, TestYieldThenRunFromDeeperLayer)
{
    StackfullTasks::Tasklet tasklet(TestTaskletYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    TestTaskletRun(tasklet);

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(TaskletTest, TestSingleTaskletWithBase)
{
    StackfullTasks::Tasklet tasklet(TestTaskletYeildFunctionWithBase);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, -1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 2);

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::FINISHED);

}

TEST_F(TaskletTest, TestCreateAndRunTaskletInsideTasklet)
{
    StackfullTasks::Tasklet tasklet(TestCreateAndRunTasklet);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 3);

}

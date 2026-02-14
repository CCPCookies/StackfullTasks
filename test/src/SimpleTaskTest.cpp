
#include "TestFixture.h"

#include <SimpleTask.h>

struct SimpleTaskTest : public TestFixture
{
};

// Test functions
void TestTaskEmpyFunction(StackfullTasks::Tasklet* tasklet)
{
    TestFixture::s_testInt = 1;
}

void TestTaskYeildFunction(StackfullTasks::Tasklet* tasklet)
{
    TestFixture::s_testInt = 1;

    tasklet->Yield();

    TestFixture::s_testInt = 2;
}

void TestTaskMultiYeildFunction(StackfullTasks::Tasklet* tasklet)
{
    TestFixture::s_testInt = 1;

    tasklet->Yield();

    TestFixture::s_testInt = 2;

    tasklet->Yield();

    TestFixture::s_testInt = 3;
}

void TestTaskYeildFunctionWithBase(StackfullTasks::Tasklet* tasklet)
{
    TestFixture::s_testInt = -1;

    tasklet->Yield();

    TestTaskYeildFunction(tasklet);
}

void TestTaskKill(StackfullTasks::Tasklet* tasklet)
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

void TestTaskOneFunctionDeep()
{
    StackfullTasks::SimpleTask tasklet(TestTaskYeildFunction);

    EXPECT_TRUE(tasklet.Run());
    EXPECT_TRUE(tasklet.Run());

}

void TestTaskRun(StackfullTasks::Tasklet& tasklet)
{
    EXPECT_TRUE(tasklet.Run());
}

void TestCreateAndRunTask(StackfullTasks::Tasklet* activeTasklet)
{
    TestFixture::s_testInt = 1;

    activeTasklet->Yield();

    StackfullTasks::SimpleTask innerTasklet(TestTaskYeildFunction);

    EXPECT_TRUE(innerTasklet.Run());

    EXPECT_EQ(TestFixture::s_testInt, 1);

    EXPECT_TRUE(innerTasklet.Run());

    EXPECT_EQ(TestFixture::s_testInt, 2);

    TestFixture::s_testInt = 3;
}

// ---Tasklet Tests---
TEST_F(SimpleTaskTest, TestSimpleTaskSwitch)
{
    StackfullTasks::SimpleTask tasklet(TestTaskEmpyFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::FINISHED);

    EXPECT_EQ(s_testInt, 1);
}

TEST_F(SimpleTaskTest, TestSingleTaskYeild)
{
    StackfullTasks::SimpleTask tasklet(TestTaskYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::SUSPENDED);

    // Should have returned before setting s_testInt to 2
    EXPECT_EQ(s_testInt, 1);
}

TEST_F(SimpleTaskTest, TestSingleTaskYeildAndResume)
{
    StackfullTasks::SimpleTask tasklet(TestTaskYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::SUSPENDED);

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::FINISHED);

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(SimpleTaskTest, TestSingleTaskMultipleYeilds)
{
    StackfullTasks::SimpleTask tasklet(TestTaskMultiYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 2);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 3);
}

TEST_F(SimpleTaskTest, TestSingleTaskYeildAndKill)
{
    StackfullTasks::SimpleTask tasklet(TestTaskKill);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::SUSPENDED);

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Kill());

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::FINISHED);

    EXPECT_EQ(s_testInt, 3);
}

TEST_F(SimpleTaskTest, TestSingleCalledOneFunctionDeepTask)
{
    TestTaskOneFunctionDeep();

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(SimpleTaskTest, TestYieldThenRunFromDeeperLayer)
{
    StackfullTasks::SimpleTask tasklet(TestTaskYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    TestTaskRun(tasklet);

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(SimpleTaskTest, TestSingleTaskletWithBase)
{
    StackfullTasks::SimpleTask tasklet(TestTaskYeildFunctionWithBase);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, -1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 2);

    EXPECT_EQ(tasklet.GetState(), StackfullTasks::TaskletState::FINISHED);

}

TEST_F(SimpleTaskTest, TestCreateAndRunTaskInsideTasklet)
{
    StackfullTasks::SimpleTask tasklet(TestCreateAndRunTask);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(tasklet.Run());

    EXPECT_EQ(s_testInt, 3);

}

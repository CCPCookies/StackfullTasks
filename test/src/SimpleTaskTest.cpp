
#include "TestFixture.h"

#include <SimpleTask.h>

struct SimpleTaskTest : public TestFixture
{
};

// Test functions
void TestTaskEmpyFunction(StackfullTasks::Fibre* fibre)
{
    TestFixture::s_testInt = 1;
}

void TestTaskYeildFunction(StackfullTasks::Fibre* fibre)
{
    TestFixture::s_testInt = 1;

    fibre->Yield();

    TestFixture::s_testInt = 2;
}

void TestTaskMultiYeildFunction(StackfullTasks::Fibre* fibre)
{
    TestFixture::s_testInt = 1;

    fibre->Yield();

    TestFixture::s_testInt = 2;

    fibre->Yield();

    TestFixture::s_testInt = 3;
}

void TestTaskYeildFunctionWithBase(StackfullTasks::Fibre* fibre)
{
    TestFixture::s_testInt = -1;

    fibre->Yield();

    TestTaskYeildFunction(fibre);
}

void TestTaskKill(StackfullTasks::Fibre* fibre)
{
    TestFixture::s_testInt = 1;
    
    if (fibre->Yield())
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
    StackfullTasks::SimpleTask task(TestTaskYeildFunction);

    EXPECT_TRUE(task.Run());
    EXPECT_TRUE(task.Run());

}

void TestTaskRun(StackfullTasks::Fibre& fibre)
{
    EXPECT_TRUE(fibre.Run());
}

void TestCreateAndRunTask(StackfullTasks::Fibre* activeFibre)
{
    TestFixture::s_testInt = 1;

    activeFibre->Yield();

    StackfullTasks::SimpleTask innerTask(TestTaskYeildFunction);

    EXPECT_TRUE(innerTask.Run());

    EXPECT_EQ(TestFixture::s_testInt, 1);

    EXPECT_TRUE(innerTask.Run());

    EXPECT_EQ(TestFixture::s_testInt, 2);

    TestFixture::s_testInt = 3;
}

// ---Task Tests---
TEST_F(SimpleTaskTest, TestSimpleTaskSwitch)
{
    StackfullTasks::SimpleTask task(TestTaskEmpyFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(task.GetState(), StackfullTasks::FibreState::FINISHED);

    EXPECT_EQ(s_testInt, 1);
}

TEST_F(SimpleTaskTest, TestSingleTaskYeild)
{
    StackfullTasks::SimpleTask task(TestTaskYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(task.GetState(), StackfullTasks::FibreState::SUSPENDED);

    // Should have returned before setting s_testInt to 2
    EXPECT_EQ(s_testInt, 1);
}

TEST_F(SimpleTaskTest, TestSingleTaskYeildAndResume)
{
    StackfullTasks::SimpleTask task(TestTaskYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(task.GetState(), StackfullTasks::FibreState::SUSPENDED);

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(task.GetState(), StackfullTasks::FibreState::FINISHED);

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(SimpleTaskTest, TestSingleTaskMultipleYeilds)
{
    StackfullTasks::SimpleTask task(TestTaskMultiYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(s_testInt, 2);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(s_testInt, 3);
}

TEST_F(SimpleTaskTest, TestSingleTaskYeildAndKill)
{
    StackfullTasks::SimpleTask task(TestTaskKill);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(task.GetState(), StackfullTasks::FibreState::SUSPENDED);

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(task.Kill());

    EXPECT_EQ(task.GetState(), StackfullTasks::FibreState::FINISHED);

    EXPECT_EQ(s_testInt, 3);
}

TEST_F(SimpleTaskTest, TestSingleCalledOneFunctionDeepTask)
{
    TestTaskOneFunctionDeep();

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(SimpleTaskTest, TestYieldThenRunFromDeeperLayer)
{
    StackfullTasks::SimpleTask task(TestTaskYeildFunction);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(s_testInt, 1);

    TestTaskRun(task);

    EXPECT_EQ(s_testInt, 2);
}

TEST_F(SimpleTaskTest, TestSingleTaskWithBase)
{
    StackfullTasks::SimpleTask task(TestTaskYeildFunctionWithBase);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(s_testInt, -1);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(s_testInt, 2);

    EXPECT_EQ(task.GetState(), StackfullTasks::FibreState::FINISHED);

}

TEST_F(SimpleTaskTest, TestCreateAndRunTaskInsideTask)
{
    StackfullTasks::SimpleTask task(TestCreateAndRunTask);

    EXPECT_EQ(s_testInt, 0);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(s_testInt, 1);

    EXPECT_TRUE(task.Run());

    EXPECT_EQ(s_testInt, 3);

}

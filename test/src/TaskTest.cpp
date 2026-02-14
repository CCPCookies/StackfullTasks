
#include "TestFixture.h"

#include <Task.h>

struct TaskTest : public TestFixture
{
};

// Test functions

std::string TestTaskWithArgumentsAndReturnFunction(StackfullTasks::Tasklet* tasklet, std::string a, std::string b)
{
    std::string c = a + b;

    TestFixture::s_testInt = 1;

    tasklet->Yield();

    TestFixture::s_testInt = 2;

    return c;
}

TEST_F(TaskTest, TestTaskWithArgumentsAndReturn)
{
    std::string a = "Hello";
    std::string b = "World";

    StackfullTasks::Task<std::string, std::string, std::string> task(TestTaskWithArgumentsAndReturnFunction);

    task.Bind(a, b);

    EXPECT_TRUE( task.Run() );

    EXPECT_EQ(task.GetState(), StackfullTasks::TaskletState::SUSPENDED);

    EXPECT_EQ(TestFixture::s_testInt, 1);

    EXPECT_TRUE( task.Run() );

    EXPECT_EQ(task.GetState(), StackfullTasks::TaskletState::FINISHED);

    EXPECT_EQ(TestFixture::s_testInt, 2);

    std::string stringReturn = task.GetReturnValue();

    EXPECT_EQ(stringReturn, a + b);
}

bool TestTaskletYeildFunction(StackfullTasks::Tasklet* tasklet, int a, int b)
{
    TestFixture::s_testInt = a;

    tasklet->Yield();

    TestFixture::s_testInt = b;

    return true;
}

bool TestTaskletSetParentFunction(StackfullTasks::Tasklet* tasklet, StackfullTasks::Tasklet* newParent, int a, int b)
{
    tasklet->SetParent(newParent);

    TestFixture::s_testInt = a;

    tasklet->Yield();

    TestFixture::s_testInt = b;

    return true;
}

TEST_F(TaskTest, TestTaskWithParentChange)
{
    // Run to yield and get back to main
    StackfullTasks::Task<bool, int, int> task1(TestTaskletYeildFunction);

    int a = 1;
    int b = 2;

    task1.Bind(a, b);

    EXPECT_TRUE(task1.Run());

    EXPECT_EQ(TestFixture::s_testInt, a);

    // Run another from main (parent is then main), pass in the first and internally change parent
    // Then when yield is hit it will switch to task1 rather than back to main which was the origional
    // parent
    StackfullTasks::Task<bool, StackfullTasks::Tasklet*, int, int > task2(TestTaskletSetParentFunction);

    int c = 3;
    int d = 4;

    task2.Bind(&task1, c, d);

    EXPECT_TRUE(task2.Run());

    EXPECT_EQ(TestFixture::s_testInt, b);
}
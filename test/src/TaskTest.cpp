
#include "TestFixture.h"
#include <Task.h>
#include <Tasklet.h>

struct TaskTest : public TestFixture
{
};

// Test functions

std::string TestTaskWithArgumentsAndReturnFunction(Tasklet* tasklet, std::string a, std::string b)
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

    Task<std::string, std::string, std::string> task(TestTaskWithArgumentsAndReturnFunction);

    task.Bind(a, b);

    EXPECT_TRUE( task.Run() );

    EXPECT_EQ(task.GetState(), TaskletState::SUSPENDED);

    EXPECT_EQ(TestFixture::s_testInt, 1);

    EXPECT_TRUE( task.Run() );

    EXPECT_EQ(task.GetState(), TaskletState::FINISHED);

    EXPECT_EQ(TestFixture::s_testInt, 2);

    std::string stringReturn = task.GetReturnValue();

    EXPECT_EQ(stringReturn, a + b);
}
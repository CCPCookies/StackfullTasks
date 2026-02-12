
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

    tasklet->Yield();

    return c;
}

TEST_F(TaskTest, TestTaskWithArgumentsAndReturn)
{
    std::string a = "Hello";
    std::string b = "World";

    Task<std::string, std::string, std::string> stringVariadic(TestTaskWithArgumentsAndReturnFunction);

    stringVariadic.Bind(a, b);

    bool res = stringVariadic.Run();

    while (!stringVariadic.IsFinished())
    {
        res = stringVariadic.Run();
    }

    std::string stringReturn = stringVariadic.GetReturnValue();

    EXPECT_EQ(stringReturn, a + b);
}
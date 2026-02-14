# StackfullTasks

Run user defined tasks to run on their own separate stack with stack switching orchestration.

The library offers the ability for functions to yield control at any point in the jobs execution to be resumed later.

**Note:** This project is in active development although the core of the functionality is available. This documentation does not yet cover the whole functionality of the system but should provide enough to use the system.

## Compatibility
Currently only x86-64 Windows systems are compatible.
I aim to support arm in the future.

## Installation

The project uses cmake build system. CMakeLists.txt is located in the project root.
Dependencies are managed by Microsoft Vcpkg which needs to be available.
Refer to `CMakeUserPresets[EXAMPLE].json`.

## Example Usage

There are two Task types currently implemented.

### QuickStart - Creating a Simple Task

First we create a function that the simple task will run.
A simple task must follow the same function signature:

```c++
void SimpleTask(Tasklet* tasklet)
{
    // work goes here
    std::cout << "Hello" << std::endl;

    std::cout << "World" << std::endl;
}
```

A simple task can then be created and run as follows.

```c++
    // Create Task bound the function TestCreateAndRunTask
    StackfullTasks::SimpleTask task(TestCreateAndRunTask);

    // Run the Task
    task.Run();
```

The output will then give.

```
    Hello
    World
```

As it stands this is an elaborate way to call a function, but the special thing here is that the job executed on it's own stack.

The power becomes more obvious when we use some of the special orchestration functions.

For example, if we add a yield to the `SimpleTask` function:

```c++
void SimpleTask(Tasklet* tasklet)
{
    // work goes here
    std::cout << "Hello" << std::endl;

    tasklet->Yield();

    std::cout << "World" << std::endl;
}
```

Then the execution of the job will now stop at `tasklet->Yield();`.

So now when running the below code we get a different output.

```c++
    StackfullTasks::SimpleTask task(TestCreateAndRunTask);
    task.Run();
```

Now gives the output of:

```
    Hello
```

But what happened to the rest?

The task is not complete and is now in a `SUSPENDED` state. See `StackfullTasks::Tasklet::GetState` to query a task's state.

A `SUSPENDED` can be resumed by simply running it again.

```c++
    task.Run();
```
Execution will continue where it yielded and so the output will give:
```
    World
```

### Complex function signature

Tasks with complex function signatures are supported via `StackfullTasks::Task`. This structure is still under development but is functional.

These functions currently support all non-void return types and must use 1 or more additional arguments after `StackfullTasks::Tasklet* tasklet`.

```c++
std::string addStrings(StackfullTasks::Tasklet* tasklet, std::string a, std::string b)
{
    std::string c = a + b;

    tasklet->Yield();

    return c;
}
```

The example function which will add two `std::strings`, yield, and then when resumed will return the result.

```c++

// Create the Task supplying <ReturnType, 1- n Argument types>
StackfullTasks::Task<std::string, std::string, std::string> task(TestTaskWithArgumentsAndReturnFunction);

// Bind the argument values to be passed the job
task.bind("hello","world");

// First run of the task which will run to Yield
task.Run();

// Second run of the task to complete the job
task.Run();

// Retreive result
std::string returnValue = task.GetReturnValue();

```
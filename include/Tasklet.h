#pragma once

#include <functional>

class Tasklet;

extern "C" int RunTaskletASM( Tasklet* tasklet );

extern "C" void YieldTaskletASM();

extern "C" int ResumeTaskletASM();

#define TASKLET_STATE_NOT_STARTED_CODE 0
#define TASKLET_STATE_SUSPENDED_CODE 1
#define TASKLET_STATE_FINISHED_CODE 2
#define TASKLET_STATE_KILLED_CODE 3
#define TASKLET_STATE_ERROR_CODE 4
#define TASKLET_STATE_UNKNOWN_CODE 5

enum class TaskletState
{
	NOT_STARTED,
	SUSPENDED,
	FINISHED,
	KILLED,
	ERROR,
	UNKNOWN,
};

class Tasklet
{
public:

	Tasklet();

	Tasklet(std::function<void(Tasklet* coroutine)> function);

	~Tasklet();

	bool Run();

	bool Yield();

	bool Kill();

	TaskletState GetState();

	virtual void RunFunction();

	bool IsFinished();

	bool SetParent(Tasklet* parent);

private:

	void UpdateStatusFromCode(int statusCode);

private:


	TaskletState m_state;

	std::function<void(Tasklet* coroutine)> m_function;

};

extern "C" void TaskletFunctionEntry(Tasklet* activeTasklet);
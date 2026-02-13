#pragma once

#include <functional>

namespace StackfullTasks
{

	class Tasklet;

	extern "C" int RunTaskletASM(Tasklet* activeTasklet, unsigned __int64* stackMemory);

	extern "C" void YieldTaskletASM(unsigned __int64* parentRsp, unsigned __int64* rspAtYield);

	extern "C" int ResumeTaskletASM(unsigned __int64* parentRsp, unsigned __int64* rspAtYield);

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

		Tasklet(std::function<void(Tasklet* coroutine)> function, unsigned int StackSize = 1000);

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

		unsigned __int64* m_stackMemory;

		unsigned __int64* m_stackMemoryStart;

		unsigned __int64* m_parentRsp;

		unsigned __int64 m_rspAtYield;

	};

	extern "C" void TaskletFunctionEntry(Tasklet* activeTasklet);

}
#pragma once

#include <functional>

namespace StackfullTasks
{

	class Fibre;

	extern "C" int RunFibreASM(Fibre* activeFibre, unsigned __int64* stackMemory);

	extern "C" void YieldFibreASM(unsigned __int64* parentRsp, unsigned __int64* rspAtYield);

	extern "C" int ResumeFibreASM(unsigned __int64* parentRsp, unsigned __int64* rspAtYield);

	#define FIBRE_STATE_NOT_STARTED_CODE 0
	#define FIBRE_STATE_SUSPENDED_CODE 1
	#define FIBRE_STATE_FINISHED_CODE 2
	#define FIBRE_STATE_KILLED_CODE 3
	#define FIBRE_STATE_ERROR_CODE 4
	#define FIBRE_STATE_UNKNOWN_CODE 5

	enum class FibreState
	{
		NOT_STARTED,
		SUSPENDED,
		FINISHED,
		KILLED,
		ERROR,
		UNKNOWN,
	};

	class Fibre
	{
	public:

		Fibre(unsigned int StackSize = 1000);

		~Fibre();

		bool Run();

		bool Yield();

		bool Kill();

		FibreState GetState();

		bool IsFinished();

		bool SetParent(Fibre* parent);

		virtual void RunFunction() = 0;

	private:

		void UpdateStatusFromCode(int statusCode);

	private:


		FibreState m_state;

		unsigned __int64* m_stackMemory;

		unsigned __int64* m_stackMemoryStart;

		unsigned __int64* m_parentRsp;

		unsigned __int64 m_rspAtYield;

	};

	extern "C" void FibreFunctionEntry(Fibre* activeFibre);

}
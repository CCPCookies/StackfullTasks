#include "Fibre.h"

#include <sstream>

namespace StackfullTasks
{

	Fibre::Fibre( unsigned int StackSize/* = 1000*/) :
		m_state(FibreState::NOT_STARTED),
		m_stackMemory(new unsigned __int64[StackSize]),
		m_stackMemoryStart(&m_stackMemory[StackSize - 1]),
		m_parentRsp(&m_stackMemory[StackSize - 2])	// Stack is upside down
	{

	}

	Fibre::~Fibre()
	{
		delete[] m_stackMemory;
	}

	extern "C" void FibreFunctionEntry(Fibre* activeFibre)
	{
		activeFibre->RunFunction();

		return;
	}

	void Fibre::UpdateStatusFromCode(int statusCode)
	{
		switch (statusCode)
		{
		case FIBRE_STATE_NOT_STARTED_CODE:
		{
			m_state = FibreState::NOT_STARTED;
			break;
		}
		case FIBRE_STATE_SUSPENDED_CODE:
		{
			m_state = FibreState::SUSPENDED;
			break;
		}
		case FIBRE_STATE_FINISHED_CODE:
		{
			m_state = FibreState::FINISHED;
			break;
		}
		case FIBRE_STATE_KILLED_CODE:
		{
			m_state = FibreState::KILLED;
			break;
		}
		case FIBRE_STATE_ERROR_CODE:
		{
			m_state = FibreState::ERROR;
			break;
		}
		default:
		{
			m_state = FibreState::UNKNOWN;
		}
		}
	}

	bool Fibre::Run()
	{

		switch (m_state)
		{
		case FibreState::NOT_STARTED:
		{
			int fibreReturnStatus = RunFibreASM(this, m_stackMemoryStart);

			UpdateStatusFromCode(fibreReturnStatus);

			break;
		}

		case FibreState::KILLED:
		case FibreState::SUSPENDED:
		{
			int fibreReturnStatus = ResumeFibreASM(m_parentRsp, &m_rspAtYield);

			UpdateStatusFromCode(fibreReturnStatus);

			break;
		}

		case FibreState::FINISHED:
		{
			// Already finished
			return false;
		}

		case FibreState::ERROR:
		{
			return false;
		}

		default:
		{
			// Unknown also matches as error
			return false;
		}
		}

		return true;

	}

	bool Fibre::Yield()
	{
		// The start of a Fibre's stack contains the parent's stack pointer
		YieldFibreASM(m_parentRsp, &m_rspAtYield);

		return m_state != FibreState::KILLED;
	}

	bool Fibre::Kill()
	{
		switch (m_state)
		{
		case FibreState::NOT_STARTED:
		{
			m_state = FibreState::FINISHED;
			return true;
		}
		case FibreState::KILLED:
		{
			return true;
		}
		case FibreState::SUSPENDED:
		{
			m_state = FibreState::KILLED;

			return Run();
		}
		case FibreState::FINISHED:
		{
			return false;
		}

		case FibreState::ERROR:
		{
			return false;
		}

		default:
		{
			return false;
		}

		}

	}

	bool Fibre::IsFinished()
	{
		return m_state == FibreState::FINISHED;
	}

	FibreState Fibre::GetState()
	{
		return m_state;
	}

	bool Fibre::SetParent(Fibre* parent)
	{
		// Only update if a valid new parent has been passed in.
		// The fibre must have already started for a parent change
		// to make sense.
		if ((parent->GetState() == FibreState::NOT_STARTED)
			|| (parent->GetState() == FibreState::FINISHED)
			|| (parent->GetState() == FibreState::ERROR)
			|| (parent->GetState() == FibreState::UNKNOWN))
		{
			return false;
		}

		*m_parentRsp = parent->m_rspAtYield;

		return true;
	}

}
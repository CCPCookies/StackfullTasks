#include "Tasklet.h"

#include <sstream>

Tasklet::Tasklet(std::function<void(Tasklet* coroutine)> function, unsigned int StackSize/* = 1000*/) :
	m_function(function),
	m_state(TaskletState::NOT_STARTED),
	m_stackMemory(new unsigned __int64[StackSize]), 
	m_stackMemoryStart( &m_stackMemory[StackSize-1]),
	m_parentRsp(&m_stackMemory[StackSize - 2])	// Stack is upside down
{

}

Tasklet::~Tasklet()
{
	delete[] m_stackMemory;
}

void Tasklet::RunFunction()
{
	m_function(this);
}

extern "C" void TaskletFunctionEntry(Tasklet* activeTasklet)
{
	activeTasklet->RunFunction();
	
	return;
}

void Tasklet::UpdateStatusFromCode(int statusCode)
{
	switch (statusCode)
	{
		case TASKLET_STATE_NOT_STARTED_CODE:
		{
			m_state = TaskletState::NOT_STARTED;
			break;
		}
		case TASKLET_STATE_SUSPENDED_CODE:
		{
			m_state = TaskletState::SUSPENDED;
			break;
		}
		case TASKLET_STATE_FINISHED_CODE:
		{
			m_state = TaskletState::FINISHED;
			break;
		}
		case TASKLET_STATE_KILLED_CODE:
		{
			m_state = TaskletState::KILLED;
			break;
		}
		case TASKLET_STATE_ERROR_CODE:
		{
			m_state = TaskletState::ERROR;
			break;
		}
		default:
		{
			m_state = TaskletState::UNKNOWN;
		}
	}
}

bool Tasklet::Run()
{

	switch (m_state)
	{
		case TaskletState::NOT_STARTED:
		{
			int taskletReturnStatus = RunTaskletASM( this, m_stackMemoryStart);

			UpdateStatusFromCode(taskletReturnStatus);

			break;
		}

		case TaskletState::KILLED:
		case TaskletState::SUSPENDED:
		{
			int taskletReturnStatus = ResumeTaskletASM( m_parentRsp, &m_rspAtYield);

			UpdateStatusFromCode(taskletReturnStatus);

			break;
		}

		case TaskletState::FINISHED:
		{
			// Already finished
			return false;
		}

		case TaskletState::ERROR:
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

bool Tasklet::Yield()
{
	// The start of a Tasklet's stack contains the parent's stack pointer
	YieldTaskletASM( m_parentRsp, &m_rspAtYield);

	return m_state != TaskletState::KILLED;
}

bool Tasklet::Kill()
{
	switch (m_state)
	{
	case TaskletState::NOT_STARTED:
	{
		m_state = TaskletState::FINISHED;
		return true;
	}
	case TaskletState::KILLED:
	{
		return true;
	}
	case TaskletState::SUSPENDED:
	{
		m_state = TaskletState::KILLED;

		return Run();
	}
	case TaskletState::FINISHED:
	{
		return false;
	}

	case TaskletState::ERROR:
	{
		return false;
	}

	default:
	{
		return false;
	}

	}

}

bool Tasklet::IsFinished()
{
	return m_state == TaskletState::FINISHED;
}

TaskletState Tasklet::GetState()
{
	return m_state;
}

bool Tasklet::SetParent(Tasklet* parent)
{
	// Only update if a valid new parent has been passed in.
	// The tasklet must have already started for a parent change
	// to make sense.
	if ((parent->GetState() == TaskletState::NOT_STARTED)
		|| (parent->GetState() == TaskletState::FINISHED)
		|| (parent->GetState() == TaskletState::ERROR)
		|| (parent->GetState() == TaskletState::UNKNOWN))
	{
		return false;
	}

	*m_parentRsp = parent->m_rspAtYield;

	return true;
}
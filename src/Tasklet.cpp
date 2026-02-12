#include "Tasklet.h"

#include <sstream>


Tasklet::Tasklet():
	m_state(TaskletState::NOT_STARTED)
{
};

Tasklet::Tasklet(std::function<void(Tasklet* coroutine)> function):
	m_function(function),
	m_state(TaskletState::NOT_STARTED)
{
}

Tasklet::~Tasklet()
{

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
			int taskletReturnStatus = RunTaskletASM(this);

			UpdateStatusFromCode(taskletReturnStatus);

			break;
		}

		case TaskletState::KILLED:
		case TaskletState::SUSPENDED:
		{
			int taskletReturnStatus = ResumeTaskletASM();

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
	}

	return true;

}

bool Tasklet::Yield()
{
	YieldTaskletASM();

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
	return false;
}
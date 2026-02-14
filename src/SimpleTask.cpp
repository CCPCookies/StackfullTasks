#pragma once

#include "SimpleTask.h"

namespace StackfullTasks
{

	SimpleTask::SimpleTask(std::function<void(Tasklet* coroutine)> function):
		Tasklet(),
		m_function(function)
	{

	}

	SimpleTask::~SimpleTask()
	{

	}

	void SimpleTask::RunFunction()
	{
		m_function(this);
	}

}
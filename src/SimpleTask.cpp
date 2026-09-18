#pragma once

#include "SimpleTask.h"

namespace StackfullTasks
{

	SimpleTask::SimpleTask(std::function<void(Fibre* coroutine)> function):
		Fibre(),
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
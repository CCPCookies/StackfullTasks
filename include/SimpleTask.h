#pragma once

#include "Tasklet.h"

namespace StackfullTasks
{

	class SimpleTask : public Tasklet
	{
	public:

		SimpleTask(std::function<void(Tasklet* coroutine)> function);

		~SimpleTask();

		virtual void RunFunction() override;

	private:

		std::function<void(Tasklet* coroutine)> m_function;

	};

}
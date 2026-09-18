#pragma once

#include "Fibre.h"

namespace StackfullTasks
{

	class SimpleTask : public Fibre
	{
	public:

		SimpleTask(std::function<void(Fibre* coroutine)> function);

		~SimpleTask();

		virtual void RunFunction() override;

	private:

		std::function<void(Fibre* coroutine)> m_function;

	};

}
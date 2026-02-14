#pragma once

#include "Tasklet.h"

namespace StackfullTasks
{

	template <typename ReturnType, class... ArgumentTypes>
	class Task : public Tasklet
	{
	public:

		Task(std::function<ReturnType(Tasklet*, ArgumentTypes... values)> function) :
			Tasklet(),
			m_function(function)
		{

		}

		void Bind(ArgumentTypes... arguments)
		{
			m_arguments = std::make_tuple(arguments...);
		}

		ReturnType GetReturnValue()
		{
			return m_return;
		}

		virtual void RunFunction() override
		{
			m_return = RunInternal();
		}

	private:

		ReturnType RunInternal()
		{
			return std::apply([this](auto &&... args) { return m_function(this, args...); }, m_arguments);
		}

	private:

		// Function Arguments
		std::function<ReturnType(Tasklet*, ArgumentTypes... values)> m_function;
		std::tuple<ArgumentTypes...> m_arguments;
		ReturnType m_return;

	};

}
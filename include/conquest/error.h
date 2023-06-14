#pragma once

#include <stdexcept>
#include <string>
#include <sstream>

namespace conquest {

	class ConquestError : public std::runtime_error
	{
	public:
		ConquestError(const std::string& message)
			: std::runtime_error(message)
		{}

		template<typename... Message>
		static ConquestError make(Message&&... args)
		{
			std::ostringstream stream;
			(stream << ... << args);

			return ConquestError(stream.str());
		}
	};
}
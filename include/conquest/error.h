#pragma once

#include <stdexcept>
#include <string>


namespace conquest {

	class ConquestError : public std::runtime_error
	{
	public:
		ConquestError(const std::string& message)
			: std::runtime_error(message)
		{}
	};
}
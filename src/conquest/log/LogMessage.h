#pragma once

#include <variant>
#include <string>
#include <sstream>
#include <chrono>


namespace conquest::log {

using MessageParameter = std::variant<std::string, int, float, double>;

enum class Severity
{
	INFO,
	WARNING,
	ERROR,
	FATAL,
};

struct LogMessage
{
	Severity severity;
	std::string message;
	std::chrono::system_clock::time_point timestamp;
	std::stringstream parameters;
};

}

#pragma once

#include <utility>

#include "conquest/common/Finally.h"
#include "conquest/log/LogMessage.h"

#include <functional>


namespace conquest::log {

class LogMessageBuilder
{
public:
	explicit LogMessageBuilder(Severity severity, std::string message);

	LogMessageBuilder(const LogMessageBuilder&) = delete;
	LogMessageBuilder& operator=(const LogMessageBuilder&) = delete;

	LogMessageBuilder(LogMessageBuilder&&) = delete;
	LogMessageBuilder& operator=(LogMessageBuilder&&) = delete;

	~LogMessageBuilder();

	template<typename T>
	LogMessageBuilder& param(const std::string& name, const T& value)
	{
		if(!m_Message.parameters.view().empty()) {
			m_Message.parameters << ", ";
		}

		m_Message.parameters << name << ": " << value;
		return *this;
	}

private:
	LogMessage m_Message;
};

}

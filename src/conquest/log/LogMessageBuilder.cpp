#include "conquest/log/Log.h"

#include "conquest/log/LogMessageBuilder.h"

#include <algorithm>


namespace conquest::log {

LogMessageBuilder::LogMessageBuilder(const Severity severity, std::string message)
	: m_Message{ severity, std::move(message), std::chrono::system_clock::now(), {} }
{
    
}

LogMessageBuilder::~LogMessageBuilder()
{
	std::ranges::for_each(g_Sinks, [&](const auto& sink){
		sink->write(m_Message);
	});
}
}

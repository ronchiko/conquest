#include "conquest/log/Log.h"


namespace conquest::log {

void initialize()
{
	g_Sinks.clear();
}

void addSink(std::shared_ptr<ILogSink> sink)
{
	g_Sinks.push_back(std::move(sink));
}

LogMessageBuilder log(const Severity severity, std::string message)
{
	return LogMessageBuilder(severity, std::move(message));
}

}

#include "conquest/log/sink/LogFileSink.h"

namespace conquest::log::sink {

namespace {

std::string severityToString(const Severity severity)
{
	switch(severity) {
	case Severity::INFO:
		return "INFO";
	case Severity::WARNING:
		return "WARNING";
	case Severity::ERROR:
		return "ERROR";
	case Severity::FATAL:
		return "FATAL";
	default:
		return "UNKNOWN";
	}
}

}

LogFileSink::LogFileSink(const std::filesystem::path& filePath)
	: m_LogFile(filePath, std::ios::out)
{}

void LogFileSink::write(const LogMessage &message)
{
	std::stringstream stream;
	stream << "[" << message.timestamp << "|" << severityToString(message.severity) << "] " << message.message;

	if(!message.parameters.view().empty()) {
		stream << ":: " << message.parameters.str();
	}

	m_LogFile << stream.str() << std::endl;
}

}

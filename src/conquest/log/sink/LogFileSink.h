#pragma once

#include <filesystem>
#include <fstream>

#include "conquest/log/ILogSink.h"


namespace conquest::log::sink {

class LogFileSink : public ILogSink
{
public:
	explicit LogFileSink(const std::filesystem::path& filePath);

	void write(const LogMessage &message) override;

private:
	std::ofstream m_LogFile;
};

}
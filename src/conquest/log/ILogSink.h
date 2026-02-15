#pragma once

#include "conquest/log/LogMessage.h"


namespace conquest::log {

class ILogSink
{
public:
	virtual ~ILogSink() = default;

	/**
	   Serializes and writes a log message to the sink.

	   @param message - The log message to be written.
	 */
	virtual void write(const LogMessage& message) = 0;
};

}

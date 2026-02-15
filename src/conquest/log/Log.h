#pragma once

#include <memory>

#include "conquest/log/LogMessage.h"
#include "conquest/log/ILogSink.h"
#include "conquest/log/LogMessageBuilder.h"




namespace conquest::log {

inline std::vector<std::shared_ptr<ILogSink>> g_Sinks = {};

/**
   Initializes the logger.
 */
void initialize();

/**
   Adds a log sink to the logger.
 */
void addSink(std::shared_ptr<ILogSink> sink);

LogMessageBuilder log(Severity severity, std::string message);

// clang-format off
inline LogMessageBuilder info(std::string message) { return log(Severity::INFO, std::move(message)); }
inline LogMessageBuilder warn(std::string message) { return log(Severity::WARNING, std::move(message)); }
inline LogMessageBuilder error(std::string message) { return log(Severity::ERROR, std::move(message)); }
inline LogMessageBuilder fatal(std::string message) { return log(Severity::FATAL, std::move(message)); }
// clang-format on


}

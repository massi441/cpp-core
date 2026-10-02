#ifndef NN_SWITCH

#include "Core/IO/Logger.h"

#include <iostream>

namespace ml {

void ConsoleLogger::log(ml::LogLevel level, std::string_view message) {
    std::ostream& out = level >= ml::LogLevel::Warning
        ? std::cerr
        : std::cout;

    out << '[' << ml::LogLevelData::ToString(level) << "] " << message << '\n';
}

void FileLogger::log(ml::LogLevel level, std::string_view message) {
    mFile << '[' << ml::LogLevelData::ToString(level) << "] " << message << '\n';

    if (mAlwaysFlush) {
        mFile << std::flush;
    }
}

void MultiLogger::log(ml::LogLevel level, std::string_view message) {
    for (std::unique_ptr<ml::ILogger>& logger : mLoggers) {
        logger->log(level, message);
    }
}

}

#endif

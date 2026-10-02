#pragma once

#include <format>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include "Core/Util/Enum.h"

#ifndef NN_SWITCH
#include <filesystem>
#include <fstream>
#endif

namespace ml {

DATA_ENUM(LogLevel, Trace, Debug, Info, Warning, Error, None)

// TODO: Add custom formatters, rules, and more

/**
 * A logger interface, with formatted helpers for each log level.
 */
class ILogger {
public:
    virtual void log(ml::LogLevel level, std::string_view message) = 0;

    template <typename... Args>
    void logTrace(std::format_string<Args...> fmt, Args&&... args) {
        this->log(ml::LogLevel::Trace, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void logDebug(std::format_string<Args...> fmt, Args&&... args) {
        this->log(ml::LogLevel::Debug, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void logInfo(std::format_string<Args...> fmt, Args&&... args) {
        this->log(ml::LogLevel::Info, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void logWarning(std::format_string<Args...> fmt, Args&&... args) {
        this->log(ml::LogLevel::Warning, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void logError(std::format_string<Args...> fmt, Args&&... args) {
        this->log(ml::LogLevel::Error, std::format(fmt, std::forward<Args>(args)...));
    }

    virtual ~ILogger() = default;
};

#ifndef NN_SWITCH

class ConsoleLogger : public ILogger {
public:
    void log(ml::LogLevel level, std::string_view message) override;
};

class FileLogger : public ILogger {
public:
    explicit FileLogger(const std::filesystem::path& path, bool alwaysFlush = true) {
        mFile.open(path);
        mAlwaysFlush = alwaysFlush;
    }

    bool isOpen() const {
        return mFile.is_open();
    }

    void log(ml::LogLevel level, std::string_view message) override;

private:
    std::ofstream mFile;
    bool mAlwaysFlush = true;
};

/**
 * Forwards every message to all added loggers
 */
class MultiLogger : public ILogger {
public:
    template <typename T, typename... Args>
    T& add(Args&&... args) {
        mLoggers.push_back(std::make_unique<T>(std::forward<Args>(args)...));

        return static_cast<T&>(*mLoggers.back());
    }

    void log(ml::LogLevel level, std::string_view message) override;

private:
    std::vector<std::unique_ptr<ml::ILogger>> mLoggers;
};

#endif

}

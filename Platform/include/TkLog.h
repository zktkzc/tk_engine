#ifndef TK_ENGINE_TK_LOG_H
#define TK_ENGINE_TK_LOG_H

#include "TkEngine.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "spdlog/spdlog.h"

namespace tk {
    class TkLog {
    public:
        TkLog() = delete;

        TkLog(const TkLog &) = delete;

        TkLog &operator=(const TkLog &) = delete;

        static void Init();

        static spdlog::logger *GetLoggerInstance() {
            assert(sLoggerInstance && "Logger instance is null, maybe you have not execute TkLog::Init().");
            return sLoggerInstance.get();
        }

    private:
        static std::shared_ptr<spdlog::logger> sLoggerInstance;
    };

#define LOG_T(...) SPDLOG_LOGGER_TRACE(tk::TkLog::GetLoggerInstance(), __VA_ARGS__)
#define LOG_D(...) SPDLOG_LOGGER_DEBUG(tk::TkLog::GetLoggerInstance(), __VA_ARGS__)
#define LOG_I(...) SPDLOG_LOGGER_INFO(tk::TkLog::GetLoggerInstance(), __VA_ARGS__)
#define LOG_W(...) SPDLOG_LOGGER_WARN(tk::TkLog::GetLoggerInstance(), __VA_ARGS__)
#define LOG_E(...) SPDLOG_LOGGER_ERROR(tk::TkLog::GetLoggerInstance(), __VA_ARGS__)
}

#endif //TK_ENGINE_TK_LOG_H

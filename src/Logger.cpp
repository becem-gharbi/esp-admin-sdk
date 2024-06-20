#include "Logger.h"

namespace ESPAdmin
{
    Logger::Logger(const char *scope) : _scope(scope)
    {
    }

    /**
     * Logs an informational message.
     *
     * @param format The format string for the message.
     *
     * @throws None
     */
    void Logger::info(const char *format, ...) const
    {
        va_list args;
        va_start(args, format);

        char buffer[DEFAULT_INT(Store::options.logMaxMessageSize, 200)];
        vsnprintf(buffer, sizeof(buffer), format, args);

        va_end(args);
        _log(ANSI_COLOR_BLUE, "info", buffer);
    }

    /**
     * Logs an error message with the given format and arguments.
     *
     * @param format The format string for the error message.
     * @param ... The arguments for the format string.
     *
     * @throws None
     */
    void Logger::error(const char *format, ...) const
    {
        va_list args;
        va_start(args, format);

        char buffer[DEFAULT_INT(Store::options.logMaxMessageSize, 200)];
        vsnprintf(buffer, sizeof(buffer), format, args);

        va_end(args);
        _log(ANSI_COLOR_RED, "error", buffer);
    }

    /**
     * Logs a warning message with the given format and arguments.
     *
     * @param format The format string for the warning message.
     * @param ... The arguments for the format string.
     *
     * @throws None
     */
    void Logger::warn(const char *format, ...) const
    {
        va_list args;
        va_start(args, format);

        char buffer[DEFAULT_INT(Store::options.logMaxMessageSize, 200)];
        vsnprintf(buffer, sizeof(buffer), format, args);

        va_end(args);
        _log(ANSI_COLOR_YELLOW, "warn", buffer);
    }

    /**
     * Logs a success message with the given format and arguments.
     *
     * @param format The format string for the success message.
     * @param ... The arguments for the format string.
     *
     * @throws None
     */
    void Logger::success(const char *format, ...) const
    {
        va_list args;
        va_start(args, format);

        char buffer[DEFAULT_INT(Store::options.logMaxMessageSize, 200)];
        vsnprintf(buffer, sizeof(buffer), format, args);

        va_end(args);
        _log(ANSI_COLOR_GREEN, "success", buffer);
    }

    void Logger::_log(const char *color, const char *type, const char *message) const
    {
        String fullMessage = "[" + _scope + "] " + String(message);

        if (Store::logSerialEnabled)
        {
            Serial.printf("%s %s %s %s \n", color, type, ANSI_COLOR_RESET, fullMessage.c_str());
        }
        if (Store::logRemoteEnabled)
        {
            MQTT::publish("/logs/" + String(type), fullMessage, 0, false);
        }
    }
}

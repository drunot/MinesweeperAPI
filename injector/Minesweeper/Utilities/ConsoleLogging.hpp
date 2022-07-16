#ifndef F50DF59E_9661_424F_8E6E_22D9599120FB
#define F50DF59E_9661_424F_8E6E_22D9599120FB
#ifdef DO_LOGGING
#include <vector>
#include <string>
#include <cstdio>
#include <cstdarg>
#include "CConsole.hpp"


#define LOG_INFO(format, ...) logging::info(format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) logging::warning(format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) logging::error(format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) logging::fatal(format, ##__VA_ARGS__)
#define LOG_SHOW_CONSOLE() logging::consoleShow()
#define LOG_HIDE_CONSOLE() logging::consoleHide()
/* static */
class logging {
  private:
    static CConsole* _console;
    static std::vector<std::string>* _toSend;
    static void printLogMsg(const std::string format, va_list args);
    static void printChashed();
    static std::string getMsgTime();
  public:
    logging() = delete;
    ~logging() = delete;
    logging& operator=(logging& right) = delete;
    logging& operator=(logging&& right) = delete;

    static void info(const std::string format, ...);
    static void warning(const std::string format, ...);
    static void error(const std::string format, ...);
    static void fatal(const std::string format, ...);

    static void consoleShow();
    static void consoleHide();
};
#else
#define LOG_INFO(format, ...)
#define LOG_WARNING(format, ...)
#define LOG_ERROR(format, ...)
#define LOG_FATAL(format, ...)
#define LOG_SHOW_CONSOLE()
#define LOG_HIDE_CONSOLE()

#endif // DO_LOGGING
#endif // F50DF59E_9661_424F_8E6E_22D9599120FB

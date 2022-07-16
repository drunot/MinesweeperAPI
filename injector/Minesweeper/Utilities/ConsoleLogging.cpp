#ifdef DO_LOGGING
#include "ConsoleLogging.hpp"
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

void logging::info(const std::string format, ...) {
    std::string newFormat = "[info    " + getMsgTime() + "] " + format + "\n";
    std::va_list args;
    va_start(args, format);
    logging::printLogMsg(newFormat, args);
    va_end(args);
}
void logging::warning(const std::string format, ...) {
    std::string newFormat = "[warning " + getMsgTime() + "] " + format + "\n";
    std::va_list args;
    va_start(args, format);
    logging::printLogMsg(newFormat, args);
    va_end(args);
}
void logging::error(const std::string format, ...) {
    std::string newFormat = "[error   " + getMsgTime() + "] " + format + "\n";
    std::va_list args;
    va_start(args, format);
    logging::printLogMsg(newFormat, args);
    va_end(args);
}
void logging::fatal(const std::string format, ...) {
    std::string newFormat = "[fatal   " + getMsgTime() + "] " + format + "\n";
    std::va_list args;
    va_start(args, format);
    logging::printLogMsg(newFormat, args);
    va_end(args);
}
void logging::consoleShow() {
    if(!_console) {
        _console = new CConsole();

    } else {
        _console->show();
    }

    printChashed();
}
void logging::consoleHide() {
    if(_console) {
        _console->hide();
    }
}

void logging::printChashed() {
    if(_toSend) {
        for(auto msg : *_toSend) {
            std::printf("%s", msg.c_str());
        }

        delete _toSend;
        _toSend = nullptr;
    }
}

void logging::printLogMsg(const std::string format, va_list args) {
    if(!_console) {
        // If console is not open. Cash messages.
        if(!_toSend) {
            // Allocate to send, if not done yet.
            _toSend = new std::vector<std::string>();
        }

        size_t bufsz = vsnprintf(NULL, 0, format.c_str(), args) + 1;
        char* buffer = new char[bufsz];
        //vsnprintf to the new allocated pointer.
        std::vsnprintf(buffer, bufsz, format.c_str(), args);
        _toSend->push_back(std::string(buffer));

        if(_toSend->size() > 1000) {
            _toSend->erase(_toSend->begin());
        }

    } else {
        // If console is open print all chased messages and print current message.
        printChashed();
        std::vprintf(format.c_str(), args);
        //printf("Test1!");
    }

    //printf("%s\n", format.c_str());
}

std::string logging::getMsgTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t),
                        "%F %T.") << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

CConsole* logging::_console = nullptr;
std::vector<std::string>* logging::_toSend = nullptr;
#endif // DO_LOGGING
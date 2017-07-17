#include "log.h"
#include <cstdio>
#include <ctime>

using namespace glamorous;

Log::Level Log::level_ = Log::Level::DEBUG;

void Log::set_level(Level level) {
    level_ = level;
}

void Log::debug(const char* fmt, ...) {
    if (level_ >= Level::DEBUG) {
        va_list arg_ptr;
        va_start(arg_ptr, fmt);
        log("D", fmt, arg_ptr);
        va_end(arg_ptr);
    }
}

void Log::info(const char* fmt, ...) {
    if (level_ >= Level::INFO) {
        va_list arg_ptr;
        va_start(arg_ptr, fmt);
        log("I", fmt, arg_ptr);
        va_end(arg_ptr);
    }
}

void Log::error(const char* fmt, ...) {
    if (level_ >= Level::ERROR) {
        va_list arg_ptr;
        va_start(arg_ptr, fmt);
        log("E", fmt, arg_ptr);
        va_end(arg_ptr);
    }
}

void Log::log(const char* type, const char* fmt, va_list ap) {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    char foo[24];
    strftime(foo, sizeof(foo), "[%F %T]", &tm);
    fprintf(stderr, "%s %s ", foo, type);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    fflush(stderr);
}

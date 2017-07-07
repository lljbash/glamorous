#pragma once

#include <cstdarg>

namespace glamorous {

class Log {
public:
    enum Level {
        NONE = 0,
        ERROR,
        INFO,
        DEBUG
    };
    
    static void set_level(Level level);
    
    static void debug(const char* fmt, ...);
    static void info(const char* fmt, ...);
    static void error(const char* fmt, ...);

private:
    static void log(const char* type, const char* fmt, va_list ap);
    
    static Level level_;
};

} // namespace glamorous

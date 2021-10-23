#ifndef CAEP_LOGGER_CPP
#define CAEP_LOGGER_CPP

#include "../log/Logger.h"
#include "../log/log_util.h"

namespace caep {

//Print formats using the default formats for its operands and logs the message.
template <typename T, typename... Object>
void Logger::Print(T arg, Object... objects) {

    return;
}

//Printf formats according to a format specifier and logs the message.
template <typename... Object>
void Logger::Printf(std::string format, Object... objects) {
    Print(objects...);
}

DefaultLogger LogUtil::d_logger;

} // namespace caep

#endif

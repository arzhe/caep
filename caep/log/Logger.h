#ifndef CAEP_LOGGER_H
#define CAEP_LOGGER_H

#include <string>

namespace caep {

class Logger {
protected:
    bool m_enabled;

public:
    // EnableLog controls whether print the message.
    virtual void EnableLog(bool enable) = 0;

    // IsEnabled returns if logger is enabled.
    virtual bool IsEnabled() = 0;

    // Print formats using the default formats for its operands and logs the message.
    template <typename T, typename... Object>
    void Print(T arg, Object... objects);

    // Printf formats according to a format specifier and logs the message.
    template <typename... Object>
    void Printf(std::string, Object... objects);

};

} // namespace caep 

#endif

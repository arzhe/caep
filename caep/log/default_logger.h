#ifndef CAEP_DEFAULT_LOGGER_H
#define CAEP_DEFAULT_LOGGER_H

#include "./Logger.h"

namespace caep {

class DefaultLogger : public Logger {
public:
    void EnableLog(bool enable) {
        m_enabled = enable;
    }

    bool IsEnabled() {
        return m_enabled;
    }

    template <typename... Object>
    void Print(Object... objects) {
        if(m_enabled)
            Print(objects...);
    }

    template <typename... Object>
    void Print(std::string format, Object... objects) {
        if(m_enabled)
            Printf(format, objects...);
    }
};

} // namespace caep 

#endif

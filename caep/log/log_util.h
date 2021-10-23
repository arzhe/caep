#ifndef CAEP_LOG_UTIL_H
#define CAEP_LOG_UTIL_H

#include "./default_logger.h"

namespace caep {

class LogUtil {
private:
    static DefaultLogger d_logger;

public:
    // SetLogger sets the current logger.
    static void SetLogger(const DefaultLogger& l) {
        d_logger = l;
    }

    // GetLogger returns the current logger.
    static DefaultLogger GetLogger() {
        return d_logger;
    }

    // LogPrint prints the log.
    template <typename... Object>
    static void LogPrint(Object... objects) {
        d_logger.Print(objects...);
    }

    // LogPrintf prints the log with the format.
    template <typename... Object>
    static void LogPrintf(std::string format, Object... objects) {
        d_logger.Printf(format, objects...);
    }
};

} // namespace caep 

#endif

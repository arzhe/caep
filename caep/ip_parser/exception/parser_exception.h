#ifndef CAEP_PARSER_EXCEPTION_H
#define CAEP_PARSER_EXCEPTION_H

#include <string>

namespace caep {

class ParserException {
    std::string error_message;
public:
    ParserException(std::string error_message);
};

} // namespace caep 

#endif

#ifndef CAEP_PARSER_EXCEPTION_CPP
#define CAEP_PARSER_EXCEPTION_CPP

#include "./parser_exception.h"

namespace caep {

ParserException::ParserException(std::string error_message) {
    this->error_message = error_message;
}

} // namespace caep 

#endif

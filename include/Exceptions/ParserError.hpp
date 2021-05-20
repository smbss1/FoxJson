#ifndef NTS_PARSER_EXCEPTION_HPP_
#define NTS_PARSER_EXCEPTION_HPP_

#include "AException.hpp"
#include "Lexer.h"

class ParserError : public AException
{
public:
    ParserError(const Token& oToken, const std::string &) throw();
    virtual ~ParserError() throw();
};

#endif
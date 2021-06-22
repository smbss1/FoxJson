#ifndef NTS_PARSER_EXCEPTION_HPP_
#define NTS_PARSER_EXCEPTION_HPP_

#include "AException.hpp"
#include "Lexer.h"

class ParserError : public AException
{
public:
    inline ParserError(const Token& oToken, const std::string& strMsg) noexcept
            : AException("[line " + std::to_string(oToken.m_iLinesTraversed) + "] " + strMsg) { }
    inline ~ParserError() noexcept override = default;
};

#endif
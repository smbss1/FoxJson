
#ifndef FOX_PARSER_JSON_HPP_
#define FOX_PARSER_JSON_HPP_

#include "json.hpp"
#include "Lexer.h"
#include "ParserHelper.h"

namespace fox
{
    enum json_token
    {
        OpenBracket,
        CloseBracket,
        OpenSquareBracket,
        CloseSquareBracket,
        Colon,
        SemiColon,
        Comma,
        String,
        Number_integer,
        Number_unsigned,
        Number_float,
        Identifier,
        True,
        False,
        Null,
    };

    class Parser : public ParserHelper
    {
    private:
    public:
        Parser();
        ~Parser();

        json process(const std::string& str);
        json parse();

        json parse_object();
        json parse_array();

    };
}

#endif

#ifndef FOX_PARSER_JSON_HPP_
#define FOX_PARSER_JSON_HPP_

#include "json.hpp"
#include "Lexer.h"
#include "ParserHelper.h"

namespace fox
{
    namespace json
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

            Value process(const std::string& str);
            Value parse();

            Value parse_object();
            Value parse_array();

        };
    }
}

#endif

#include <sstream>
#include "Parser.hpp"
#include "Exceptions/ParserError.hpp"

namespace fox
{
    namespace json
    {
        Parser::Parser()
        {
            olex.define("WS", "[ \t\b\r]+", true);
            olex.define(83, "[\n]", true);
            olex.define(OpenBracket, "\\{");
            olex.define(CloseBracket, "\\}");
            olex.define(OpenSquareBracket, "\\[");
            olex.define(CloseSquareBracket, "\\]");
            olex.define(Colon, ":");
            olex.define(SemiColon, ";");
            olex.define(Comma, ",");
            olex.define_area(String, '"', '"');
            olex.define(Number_integer, "[+-]?[0-9]+");
            olex.define(Number_unsigned, "[+-]?[0-9]+");
            olex.define(Number_float, "[+-]?([0-9]*)?[.][0-9]+");
            olex.define(True, "true");
            olex.define(False, "false");
            olex.define(Null, "null");
            olex.define(Identifier, "[a-zA-Z_]+[0-9]*");
        }
        
        Parser::~Parser()
        {
        }

        Value Parser::process(const std::string& str)
        {
            try
            {
                if (!Init(str))
                    throw ParserError(CurrentToken(), "Cannot parse an empty string.");
                return parse();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                return Value();
            }
        }

        Value Parser::parse()
        {
            Token token;
            if (IsTokenThenAssign(Identifier, token))
            {
                return Value(token.GetText());
            }
            else if (IsTokenThenAssign(String, token))
            {
                return Value(token.GetText());
            }
            else if (IsTokenThenAssign(Null, token))
            {
                if (PeekTokenIsType(Colon))
                    return Value(token.GetText());
                return Value(json_type::null);
            }
            else if (IsTokenThenAssign(True, token))
            {
                return Value(true);
            }
            else if (IsTokenThenAssign(False, token))
            {
                return Value(false);
            }
            else if (IsTokenThenAssign(Number_integer, token))
            {
                int v = 0;
                std::stringstream str(token.GetText());
                str >> v;
                return Value(v);
            }
            else if (IsTokenThenAssign(Number_float, token))
            {
                float v = 0;
                std::stringstream str(token.GetText());
                str >> v;
                return Value(v);
            }
            else if (IsTokenThenAssign(Number_unsigned, token))
            {
                unsigned int v = 0;
                std::stringstream str(token.GetText());
                str >> v;
                return Value(v);
            }
            else if (IsToken(OpenBracket)) // Object
            {
                return parse_object();
            }
            else if (IsToken(OpenSquareBracket)) // Array
            {
                return parse_array();
            } else
                throw ParserError(CurrentToken(), "Unknown Value '" + CurrentToken().GetText() + "'");
        }

        Value Parser::parse_object()
        {
            Object object;
            while(!IsToken(CloseBracket) && !olex.is_end())
            {
                Value key = parse();
                if (!IsToken(Colon))
                    throw ParserError(CurrentToken(), "missing ':'");

                Value value = parse();
                if (!IsToken(Comma) && !PeekTokenIsType(CloseBracket) && !olex.is_end()) {
                    std::cout << CurrentToken().GetText() << std::endl;
                    throw ParserError(CurrentToken(), "missing ','");
                }

                object.emplace(key.get<std::string>(), value);
            }
            if (olex.is_end() && PreviousToken().m_oType != CloseBracket)
                throw ParserError(PreviousToken(), "Missing '}'");
            return Value(object);
        }

        Value Parser::parse_array()
        {
            Array array;
            while(!IsToken(CloseSquareBracket) && !olex.is_end())
            {
                Value key = parse();
                if (!IsToken(Comma) && !PeekTokenIsType(CloseSquareBracket))
                    throw ParserError(CurrentToken(), "missing ','");

                array.push_back(key);
            }
            return Value(array);
        }
    }
}
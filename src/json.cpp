

#include <assert.h>
#include "json.hpp"
#include "Parser.hpp"

namespace fox
{
    namespace json
    {
        Value::~Value()
        {
            clear_internal();
        }

        Value parse(const std::string& str)
        {
            Parser parser;
            return parser.process(str);
        }

        bool Value::is_null() const
        {
            return type == json_type::null;
        }

        bool Value::is_boolean() const
        {
            return type == json_type::boolean;
        }

        bool Value::is_number() const
        {
            return type == json_type::number_integer &&
                    type == json_type::number_float &&
                    type == json_type::number_unsigned;
        }

        bool Value::is_object() const
        {
            return type == json_type::object;
        }

        bool Value::is_array() const
        {
            return type == json_type::array;
        }

        bool Value::is_string() const
        {
            return type == json_type::string;
        }

        template<>
        std::string Value::get() const
        {
            if (type == json_type::string)
                return *value.string;
            else
                return "";
        }

        template<>
        const char* Value::get() const
        {
            if (type == json_type::string)
                return value.string->c_str();
            else
                return "";
        }

        template<>
        int Value::get() const
        {
            if (type == json_type::number_integer)
                return value.number_integer;
            if (type == json_type::number_unsigned)
                return static_cast<int>(value.number_unsigned);
            if (type == json_type::number_float)
                return static_cast<int>(value.number_float);
            return 0;
        }

        template<>
        unsigned Value::get() const
        {
            if (type == json_type::number_unsigned)
                return value.number_unsigned;
            if (type == json_type::number_integer)
                return static_cast<unsigned>(value.number_integer);
            if (type == json_type::number_float)
                return static_cast<unsigned>(value.number_float);
            return 0;
        }

        template<>
        float Value::get() const
        {
            if (type == json_type::number_float)
                return value.number_float;
            if (type == json_type::number_integer)
                return static_cast<float>(value.number_integer);
            if (type == json_type::number_unsigned)
                return static_cast<float>(value.number_unsigned);
            return 0.0f;
        }

        template<>
        bool Value::get() const
        {
            if (type == json_type::boolean)
                return value.boolean;
            else return false;
        }

        template<>
        Object Value::get() const
        {
            if (type == json_type::object)
                return *value.object;
            return Object();
        }

        template<>
        Array Value::get() const
        {
            if (type == json_type::array)
                return *value.array;
            return Array();
        }

        std::string Value::indent(std::size_t depth) const
        {
            const std::size_t indent_step = 2;
            depth ? --depth : 0;
            std::string indent_str(depth * indent_step, ' ');
            return indent_str;
        }

        std::string Value::dump(std::size_t depth) const
        {
            std::string result;
            std::string indent_str = indent(depth);
            std::string indent_str_v = indent(depth ? depth + 1 : 0);
            switch (type)
            {
                case json_type::object:
                {
                    result = "{\n";
                    auto it = value.object->begin();
                    for (std::size_t i = 0; i < value.object->size() - 1; ++i, ++it)
                    {
                        result += indent_str_v;
                        result += "\"" + it->first + "\"";
                        result += ": ";
                        result += it->second.dump(depth + 1);
                        result += ",\n";
                    }

                    assert(it != value.object->end());
                    assert(std::next(it) == value.object->end());
                    result += indent_str_v + "\"" + it->first + "\"" + ": " + it->second.dump() + "\n";
                    result += indent_str + "}";
                    break;
                }

                case json_type::array:
                {
                    result = "[";
                    auto it = value.array->cbegin();
                    for (std::size_t i = 0; i < value.array->size() - 1; ++i, ++it)
                    {
                        result += it->dump();
                        result += ", ";
                    }
                    assert(it != value.array->cend());
                    assert(std::next(it) == value.array->cend());
                    result += it->dump();
                    result += "]";
                    break;
                }

                case json_type::string:
                {
                    result = "\"";
                    result += *value.string;
                    result += "\"";
                    break;
                }

                case json_type::boolean:
                {
                    result = value.boolean ? "true" : "false";
                    break;
                }

                case json_type::number_float:
                    result = std::to_string(value.number_float);
                    break;

                case json_type::number_integer:
                    result = std::to_string(value.number_integer);
                    break;

                case json_type::number_unsigned:
                    result = std::to_string(value.number_unsigned);
                    break;

                case json_type::null:
                {
                    result = "null";
                    break;
                }

                default:
                    break;
            }
            return result;
        }

        std::string Value::type_name() const
        {
            switch (type)
            {
                case json_type::object:
                    return "object";

                case json_type::array:
                    return "array";

                case json_type::string:
                    return "string";

                case json_type::boolean:
                    return "boolean";

                case json_type::number_float:
                case json_type::number_integer:
                case json_type::number_unsigned:
                    return "number";

                case json_type::null:
                    return "null";

                default:
                    return "";
            }
        }

        std::ostream& operator<<(std::ostream& os, const Value& v)
        {
            os << v.dump();
            return os;
        }
    }
}
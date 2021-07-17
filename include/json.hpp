

#ifndef FOX_JSON_HPP_
#define FOX_JSON_HPP_

#include <assert.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace fox
{
    namespace json
    {
        enum json_type
        {
            null,
            object,
            array,
            string,
            number_integer,
            number_float,
            number_unsigned,
            boolean
        };

        class Value;

        template <typename T>
        struct Serializer;

        template <typename T>
        void serialize(Value& j, const T& value);

        template <typename T>
        void deserialize(const Value& j, T& value);

        using Array = std::vector<Value>;
        using Object = std::unordered_map<std::string, Value>;
        
        class Value
        {
        private:
            union json_value
            {
                /// object (stored with pointer to save storage)
                Object* object;
                /// array (stored with pointer to save storage)
                Array* array;
                /// string (stored with pointer to save storage)
                std::string* string;
                /// boolean
                bool boolean;
                /// number (integer)
                int number_integer;
                // /// number (unsigned integer)
                unsigned number_unsigned;
                // /// number (floating-point)
                double number_float;

                /// default constructor (for null values)
                json_value() = default;
                /// constructor for booleans
                explicit json_value(bool v) noexcept : boolean(v) {}
                /// constructor for numbers (integer)
                explicit json_value(int8_t v) noexcept : number_integer(v) {}
                explicit json_value(int16_t v) noexcept : number_integer(v) {}
                explicit json_value(int32_t v) noexcept : number_integer(v) {}
                explicit json_value(int64_t v) noexcept : number_integer(v) {}
                /// constructor for numbers (unsigned)
                explicit json_value(uint8_t v) noexcept : number_unsigned(v) {}
                explicit json_value(uint16_t v) noexcept : number_unsigned(v) {}
                explicit json_value(uint32_t v) noexcept : number_unsigned(v) {}
                explicit json_value(uint64_t v) noexcept : number_unsigned(v) {}
                // /// constructor for numbers (floating-point)
                explicit json_value(float v) noexcept : number_float(v) {}
                explicit json_value(double v) noexcept : number_float(v) {}
                /// constructor for empty values of a given type
                explicit json_value(json_type t)
                {
                    object = nullptr;
                    string = nullptr;
                    array = nullptr;
                    switch (t)
                    {
                        case json_type::object:
                        {
                            object = new Object();
                            break;
                        }

                        case json_type::array:
                        {
                            array = new Array();
                            break;
                        }

                        case json_type::string:
                        {
                            string = new std::string("");
                            break;
                        }

                        case json_type::boolean:
                        {
                            boolean = false;
                            break;
                        }

                        case json_type::number_integer:
                        {
                            number_integer = 0;
                            break;
                        }

                        case json_type::number_unsigned:
                        {
                            number_unsigned = 0;
                            break;
                        }

                        case json_type::number_float:
                        {
                            number_float = 0.0f;
                            break;
                        }

                        case json_type::null:
                        {
                            break;
                        }

                        default:
                        {
                            break;
                        }
                    }
                }

                /// constructor for strings
                json_value(const std::string& value)
                {
                    string = new std::string(value);
                }

                /// constructor for strings
                json_value(const char* value)
                {
                    string = new std::string(value);
                }

                /// constructor for rvalue strings
                json_value(std::string&& value)
                {
                    string = new std::string(std::move(value));
                }

                /// constructor for objects
                json_value(const Object& value)
                {
                    object = new Object(value);
                }

                /// constructor for rvalue objects
                json_value(Object&& value)
                {
                    object = new Object(std::move(value));
                }

                /// constructor for arrays
                json_value(const Array& value)
                {
                    array = new Array(value);
                }

                /// constructor for rvalue arrays
                json_value(Array&& value)
                {
                    array = new Array(std::move(value));
                }
            };

            json_type type = json_type::null;
            json_value value;
        public:
            ~Value();
            Value() = default;
            Value(bool v) : value(v), type(json_type::boolean) {}
            Value(int8_t v) : value(v), type(json_type::number_integer) {}
            Value(int16_t v) : value(v), type(json_type::number_integer) {}
            Value(int32_t v) : value(v), type(json_type::number_integer) {}
            Value(int64_t v) : value(v), type(json_type::number_integer) {}
            Value(uint8_t v) : value(v), type(json_type::number_unsigned) {}
            Value(uint16_t v) : value(v), type(json_type::number_unsigned) {}
            Value(uint32_t v) : value(v), type(json_type::number_unsigned) {}
            Value(uint64_t v) : value(v), type(json_type::number_unsigned) {}
            Value(float v) : value(v), type(json_type::number_float) {}
            Value(double v) : value(v), type(json_type::number_float) {}
            Value(json_type t) : value(t), type(t) {}
            Value(const char* v) : value(v), type(json_type::string) {}
            Value(const std::string& v) : value(v), type(json_type::string) {}
            Value(std::string&& v) : value(v), type(json_type::string) {}
            Value(const Object& v) : value(v), type(json_type::object) {}
            Value(Object&& v) : value(v), type(json_type::object) {}
            Value(const Array& v) : value(v), type(json_type::array) {}
            Value(Array&& v) : value(v), type(json_type::array) {}
            Value(const Value& other)
            {
                switch( other.type )
                {
                case json_type::object:
                    value.object = 
                        new Object( other.value.object->begin(),
                                            other.value.object->end() );
                    break;
                case json_type::array:
                    value.array = new Array( other.value.array->begin(),
                                        other.value.array->end() );
                    break;
                case json_type::string:
                    value.string = new std::string( *other.value.string );
                    break;
                default:
                    value = other.value;
                }
                type = other.type;
            }

            template<typename T>
            Value(const T& obj)
            {
                serialize(*this, obj);
            }

            void clear_internal()
            {
                switch(type)
                {
                    case json_type::object: delete value.object;  break;
                    case json_type::array:  delete value.array;   break;
                    case json_type::string: delete value.string;  break;
                    default: break;
                }
                // type = json_type::null;
            }

            Value& operator=(Value&& other)
            {
                clear_internal();
                value = other.value;
                type = other.type;
                other.value.object = nullptr;
                other.type = json_type::null;
                return *this;
            }

            Value& operator=(const Value& other)
            {
                clear_internal();
                switch( other.type )
                {
                case json_type::object:
                    value.object = 
                        new Object( other.value.object->begin(),
                                            other.value.object->end() );
                    break;
                case json_type::array:
                    value.array = 
                        new Array( other.value.array->begin(),
                                        other.value.array->end() );
                    break;
                case json_type::string:
                    value.string = new std::string( *other.value.string );
                    break;
                default:
                    value = other.value;
                }
                type = other.type;
                return *this;
            }

            // Operator Object
            Value& operator[](const std::string& name)
            {
                if (!is_object()) {
                    clear_internal();
                    value.object = new Object();
                    type = json_type::object;
                }
                return (*value.object)[name];
            }

            Value& operator[](const std::string& name) const
            {
                // const operator[] only works for objects
                if (is_object())
                {
                    assert(value.object->find(name) != value.object->end());
                    return value.object->at(name);
                }

                throw std::runtime_error("cannot use operator[] with a string argument with " + std::string(type_name()));
            }

            // Operator Array
            Value& operator[](std::size_t i)
            {
                if (!is_array()) {
                    clear_internal();
                    value.array = new Array();
                    type = json_type::array;
                }
                
                // operator[] only works for arrays
                if (is_array())
                {
                    // fill up array with null values if given idx is outside range
                    if (i >= value.array->size())
                    {
                        value.array->resize(i + 1);
                    }
                }
                return (*value.array)[i];
            }

            Value& operator[](std::size_t i) const
            {
                if (is_array())
                {
                    return value.array->at(i);
                }

                throw std::runtime_error("cannot use operator[] with a string argument with " + std::string(type_name()));
            }

            Value& operator[](int i)
            {
                if (!is_array()) {
                    clear_internal();
                    value.array = new Array();
                    type = json_type::array;
                }

                // operator[] only works for arrays
                if (is_array())
                {
                    // fill up array with null values if given idx is outside range
                    if (i >= value.array->size())
                    {
                        value.array->resize(i + 1);
                    }
                }
                return (*value.array)[i];
            }

            Value& operator[](int i) const
            {
                if (is_array())
                {
                    return value.array->at(i);
                }

                throw std::runtime_error("cannot use operator[] with a string argument with " + std::string(type_name()));
            }

            template<typename T>
            T get() const
            {
                T new_type;
                deserialize<T>(*this, new_type);
                return new_type;
            }

            bool is_null() const;
            bool is_boolean() const;
            bool is_number() const;
            bool is_object() const;
            bool is_array() const;
            bool is_string() const;

            std::string dump(std::size_t depth = 1) const;

            private:
            std::string indent(std::size_t depth) const;
            std::string type_name() const;
        };

        Value parse(const std::string&);
    
        template<>
        std::string Value::get() const;
        template<>
        const char* Value::get() const;

        template<>
        int8_t Value::get() const;
        template<>
        int16_t Value::get() const;
        template<>
        int32_t Value::get() const;
        template<>
        int64_t Value::get() const;

        template<>
        uint8_t Value::get() const;
        template<>
        uint16_t Value::get() const;
        template<>
        uint32_t Value::get() const;
        template<>
        uint64_t Value::get() const;

        template<>
        float Value::get() const;
        template<>
        double Value::get() const;

        template<>
        bool Value::get() const;

        template<>
        Object Value::get() const;
        template<>
        Array Value::get() const;

        // Serialization
        template <typename T>
        void serialize(Value& j, const T& value)
        {
            Serializer<T>::serialize(j, value);
        }

        template <typename T>
        void deserialize(const Value& j, T& value)
        {
            Serializer<T>::deserialize(j, value);
        }

        // Serializer
        template <typename T>
        struct Serializer<std::vector<T>>
        {
            static void serialize(Value& j, const std::vector<T>& value)
            {
                std::size_t i = 0;
                for (auto& elem : value)
                {
                    j[i] = elem;
                    ++i;
                }
            }

            static void deserialize(const Value& j, std::vector<T>& value)
            {
                if (j.is_array())
                {
                    value.reserve(j.get<Array>().size()); // vector.resize() works only for default constructible types
                    for (auto& elem : j.get<Array>())
                        value.push_back(elem.get<T>()); // push rvalue
                }
            }
        };

        template <typename K, typename V>
        struct Serializer<std::unordered_map<K, V>>
        {
            static void serialize(Value& j, const std::unordered_map<K, V>& value)
            {
                for (auto& pair : value)
                {
                    Value key = pair.first;
                    Value value = pair.second;
                    j[key.get<std::string>()] = value;
                }
            }

            static void deserialize(const Value& j, std::unordered_map<K, V>& value)
            {
                if (j.is_object())
                {
                    Object obj = j.get<Object>();
                    for (auto it = obj.begin(); it != obj.end(); ++it)
                        value[it->first] = it->second.get<V>();
                }
            }
        };

        std::ostream& operator<<(std::ostream& os, const Value& v);
    }
}

#endif
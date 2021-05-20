

#ifndef FOX_JSON_HPP_
#define FOX_JSON_HPP_

#include <assert.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace fox
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

    class json;

    template <typename T>
    struct Serializer;

    template <typename T>
    void serialize(json& j, const T& value);

    template <typename T>
    void deserialize(const json& j, T& value);

    using JSONArray = std::vector<json>;
    using JSONObject = std::unordered_map<std::string, json>;
    
    class json
    {
    private:
        union json_value
        {
            /// object (stored with pointer to save storage)
            JSONObject* object;
            /// array (stored with pointer to save storage)
            JSONArray* array;
            /// string (stored with pointer to save storage)
            std::string* string;
            /// boolean
            bool boolean;
            /// number (integer)
            int number_integer;
            // /// number (unsigned integer)
            unsigned number_unsigned;
            // /// number (floating-point)
            float number_float;

            /// default constructor (for null values)
            json_value() = default;
            /// constructor for booleans
            json_value(bool v) noexcept : boolean(v) {}
            /// constructor for numbers (integer)
            json_value(int v) noexcept : number_integer(v) {}
            /// constructor for numbers (unsigned)
            json_value(unsigned v) noexcept : number_unsigned(v) {}
            // /// constructor for numbers (floating-point)
            json_value(float v) noexcept : number_float(v) {}
            /// constructor for empty values of a given type
            json_value(json_type t)
            {
                object = nullptr;
                string = nullptr;
                array = nullptr;
                switch (t)
                {
                    case json_type::object:
                    {
                        object = new JSONObject();
                        break;
                    }

                    case json_type::array:
                    {
                        array = new JSONArray();
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
            json_value(const JSONObject& value)
            {
                object = new JSONObject(value);
            }

            /// constructor for rvalue objects
            json_value(JSONObject&& value)
            {
                object = new JSONObject(std::move(value));
            }

            /// constructor for arrays
            json_value(const JSONArray& value)
            {
                array = new JSONArray(value);
            }

            /// constructor for rvalue arrays
            json_value(JSONArray&& value)
            {
                array = new JSONArray(std::move(value));
            }
        };

        json_type type = json_type::null;
        json_value value;
    public:
        ~json();
        json() = default;
        json(bool v) : value(v), type(json_type::boolean) {}
        json(int v) : value(v), type(json_type::number_integer) {}
        json(unsigned v) : value(v), type(json_type::number_unsigned) {}
        json(float v) : value(v), type(json_type::number_float) {}
        json(json_type t) : value(t), type(t) {}
        json(const char* v) : value(v), type(json_type::string) {}
        json(const std::string& v) : value(v), type(json_type::string) {}
        json(std::string&& v) : value(v), type(json_type::string) {}
        json(const JSONObject& v) : value(v), type(json_type::object) {}
        json(JSONObject&& v) : value(v), type(json_type::object) {}
        json(const JSONArray& v) : value(v), type(json_type::array) {}
        json(JSONArray&& v) : value(v), type(json_type::array) {}
        json(const json& other)
        {
            switch( other.type )
            {
            case json_type::object:
                value.object = 
                    new JSONObject( other.value.object->begin(),
                                          other.value.object->end() );
                break;
            case json_type::array:
                value.array = new JSONArray( other.value.array->begin(),
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
        json(const T& obj)
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

        json& operator=(json&& other)
        {
            clear_internal();
            value = other.value;
            type = other.type;
            other.value.object = nullptr;
            other.type = json_type::null;
            return *this;
        }

        json& operator=(const json& other)
        {
            clear_internal();
            switch( other.type )
            {
            case json_type::object:
                value.object = 
                    new JSONObject( other.value.object->begin(),
                                          other.value.object->end() );
                break;
            case json_type::array:
                value.array = 
                    new JSONArray( other.value.array->begin(),
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
        json& operator[](const std::string& name)
        {
            if (!is_object()) {
                clear_internal();
                value.object = new JSONObject();
                type = json_type::object;
            }
            return (*value.object)[name];
        }

        json& operator[](const std::string& name) const
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
        json& operator[](std::size_t i)
        {
            if (!is_array()) {
                clear_internal();
                value.array = new JSONArray();
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

        json& operator[](std::size_t i) const
        {
            if (is_array())
            {
                return value.array->at(i);
            }

            throw std::runtime_error("cannot use operator[] with a string argument with " + std::string(type_name()));
        }

        json& operator[](int i)
        {
            if (!is_array()) {
                clear_internal();
                value.array = new JSONArray();
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

        json& operator[](int i) const
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

        static json parse(const std::string&);
        std::string dump(std::size_t depth = 1) const;

        private:
        std::string indent(std::size_t depth) const;
        std::string type_name() const;
    };

    template<>
    std::string json::get() const;
    template<>
    const char* json::get() const;

    template<>
    int json::get() const;
    template<>
    unsigned json::get() const;
    template<>
    float json::get() const;

    template<>
    bool json::get() const;

    template<>
    JSONObject json::get() const;
    template<>
    JSONArray json::get() const;

    // Serialization
    template <typename T>
    void serialize(json& j, const T& value)
    {
        Serializer<T>::serialize(j, value);
        // j = meta::serialize(obj);
    }

    template <typename T>
    void deserialize(const json& j, T& value)
    {
        Serializer<T>::deserialize(j, value);
        // meta::deserialize(obj, j);
    }

    // template <typename T>
    // struct Serializer
    // {
    //     static void serialize(json& j, const T& value)
    //     {
    //         std::cout << "Serialize" << std::endl;
    //         // calls the "to_json" method in T's namespace
    //     }

    //     static void deserialize(const json& j, T& value) {
    //         std::cout << "Deserialize" << std::endl;
    //         // same thing, but with the "from_json" method
    //     }
    // };

    // Serializer
    template <typename T>
    struct Serializer<std::vector<T>>
    {
        static void serialize(json& j, const std::vector<T>& value)
        {
            std::size_t i = 0;
            for (auto& elem : value)
            {
                j[i] = elem;
                ++i;
            }
        }

        static void deserialize(const json& j, std::vector<T>& value)
        {
            if (j.is_array())
            {
                value.reserve(j.get<JSONArray>().size()); // vector.resize() works only for default constructible types
                for (auto& elem : j.get<JSONArray>())
                    value.push_back(elem.get<T>()); // push rvalue
            }
        }
    };

    template <typename K, typename V>
    struct Serializer<std::unordered_map<K, V>>
    {
        static void serialize(json& j, const std::unordered_map<K, V>& value)
        {
            std::cout << "Serialize unordered_map" << std::endl;
            // for (auto& pair : value)
            // {
            //     j.emplace(castToString(pair.first), pair.second);
            // }
        }

        static void deserialize(const json& j, std::unordered_map<K, V>& value)
        {
            std::cout << "Deserialize unordered_map" << std::endl;
            // for (auto it = j.begin(); it != j.end(); ++it)
            // {
            //     value.emplace(fromString<K>(it.key()), it.value());
            // }
        }
    };

    std::ostream& operator<<(std::ostream& os, const json& v);
}

#endif
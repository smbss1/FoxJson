
#include "json.hpp"
#include <iostream>


struct Some
{
    int data;
};

namespace fox
{
    template <>
    void json::serialize(fox::json::Value& j, const Some& obj)
    {
        // j = meta::serialize(obj);
        std::cout << "Serialize Some" << std::endl;
        j["data"] = obj.data;
    }

    template <>
    void json::deserialize(const fox::json::Value& j, Some& obj)
    {
        std::cout << "Deserialize Some" << std::endl;
        obj.data = j["data"].get<int>();
        // meta::deserialize(obj, j);
    }
}

using json = fox::json::Value;

int main()
{
    std::string str = R"(
        {
            "string": "lol",
            "number": 10,
            "neg_number": -10,
            "array": [1, 2, 3],
            "boolean": true,
            "null": null,
            "float": .10,
            "double": .10,
            "unsigned": 100,
            "obj": {
                "lol": "lol"
            }
        }
    )";
    json j = fox::json::parse(str);
    std::cout << j.get<fox::json::Object>()["string"].get<std::string>() << std::endl;
    std::cout << j << std::endl;
    Some s = {4};
    j = s;
    std::cout << j << std::endl;
    s = j.get<Some>();
    std::cout << "data: " << s.data << std::endl;

    std::vector<int> v = {0, 1, 2, 3};
    j = v;
    std::cout << j << std::endl;
    v = j.get<std::vector<int>>();
    for (auto i = v.begin(); i != v.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;

    std::vector<Some> v1 = {{0}, {1}, {2}, {3}};
    j = v1;
    std::cout << j << std::endl;

    std::unordered_map<std::string, int> o;
    o["0"] = 0;
    o["1"] = 1;
    j = o;
    std::cout << j << std::endl;
    o = j.get<std::unordered_map<std::string, int>>();
    std::cout << "{" << std::endl;
    for (auto it : o)
    {
        std::cout << it.first << ": " << it.second << std::endl;
    }
    std::cout << "}" << std::endl;
    return 0;
}
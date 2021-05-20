
#include "json.hpp"
#include <iostream>

struct Some
{
    int data;
};

namespace fox
{
    template <>
    void serialize(json& j, const Some& obj)
    {
        // j = meta::serialize(obj);
        std::cout << "Serialize Some" << std::endl;
        j["data"] = obj.data;
    }

    template <>
    void deserialize(const json& j, Some& obj)
    {
        std::cout << "Deserialize Some" << std::endl;
        obj.data = j["data"].get<int>();
        // meta::deserialize(obj, j);
    }
}

int main()
{
    std::string str = R"(
        {
            string: "lol",
            number: 10,
            neg_number: -10,
            array: [1, 2, 3],
            boolean: true,
            null: null,
            float: .10,
            unsigned: 100,
            obj: {
                lol: "lol"
            }
        }
    )";
    fox::json j = fox::json::parse(str);
    std::cout << j.get<fox::JSONObject>()["string"].get<std::string>() << std::endl;
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
    return 0;
}
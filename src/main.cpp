#include <iostream>
#include "third-party/json.hpp"

using namespace std;
using json = nlohmann::json;

int main() {
    json j2 = {
            {"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {"answer", {
                           {"everything", 42}
                   }},
            {"list", {1, 0, 2}},
            {"object", {
                           {"currency", "USD"},
                         {"value", 42.99}
                   }}
    };
    cout << j2.dump() << endl;
    return 0;
}
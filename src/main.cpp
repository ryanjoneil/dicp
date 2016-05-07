#include <fstream>
#include <iostream>
#include "third-party/json.hpp"

using namespace std;
using json = nlohmann::json;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "usage: dicp <input-dir>" << endl;
        return 1;
    }

    // Read input file
    string input_dir(argv[1]);
    string input_file = input_dir + "/input.json";
    fstream ifs { input_file };
    if (!ifs) {
        cerr << "couldn't open " << input_file << endl;
        return 1;
    }

    auto j = json::parse(ifs);
    cout << j.dump() << endl;
    return 0;
}
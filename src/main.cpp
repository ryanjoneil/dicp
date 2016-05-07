#include <iostream>
#include "dicp/DicpProblem.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "usage: dicp <input-dir>" << endl;
        return 1;
    }

    string input_dir(argv[1]);
    string input_file = input_dir + "/input.json";

    try {
        DicpProblem problem = DicpProblem::load(input_file);
        cout << problem.get_commands().size() << endl;
    } catch (dicp_load_failure dlf) {
        cerr << "can't read file '" << input_file << "'" << endl;
        return 1;
    }

    return 0;
}
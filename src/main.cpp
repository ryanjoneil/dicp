#include <iostream>
#include "dicp/DicpProblem.h"
#include "dicp/solvers/DicpSolver.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "usage: dicp <gecode-extensive> <input-dir>" << endl;
        return 1;
    }

    // Construct a solver
    string solver_name {argv[1]};
    DicpSolver* solver = nullptr;
    try {
        solver = DicpSolver::lookup(solver_name);
    } catch (dicp_invalid_solver dis) {
        cerr << "invalid solver '" << solver_name << "'" << endl;
        return 1;
    }

    // Read in the problem
    string input_dir {argv[2]};
    string input_file = input_dir + "/input.json";

    try {
        DicpProblem problem = DicpProblem::load(input_file);
        solver->solve(problem);
    } catch (dicp_load_failure dlf) {
        cerr << "can't read file '" << input_file << "'" << endl;
        return 1;
    } catch (dicp_invalid_solver dis) {
        cerr << "failed calling solver '" << solver_name << "'" << endl;
        return 1;
    } catch (dicp_solving_error dse) {
        cerr << "error during solve: '" << dse.message << "'" << endl;
        return 1;
    }

    return 0;
}
//
// Created by Ryan J. O'Neil on 5/8/16.
//

#include "DicpGecodeExtensiveSolver.h"

DicpSolver* DicpSolver::lookup(string name) {
    if (name == SOLVER_GECODE_EXTENSIVE)
        return new DicpGecodeExtensiveSolver { };
    throw dicp_invalid_solver { };
}
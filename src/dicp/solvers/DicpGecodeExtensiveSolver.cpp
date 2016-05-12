//
// Created by Ryan J. O'Neil on 5/8/16.
//

#include <gecode/search.hh>
#include "DicpGecodeExtensiveSolver.h"
#include "../spaces/DicpGecodeExtensiveSpace.h"

using namespace Gecode;

void DicpGecodeExtensiveSolver::solve(DicpProblem problem) {
    try {
        DicpGecodeExtensiveSpace *s = new DicpGecodeExtensiveSpace(problem);
        BAB <DicpGecodeExtensiveSpace> e(s);
        delete s;

        // Search & print all solutions
        while (DicpGecodeExtensiveSpace* s2 = e.next()) {
            s2->print();
            delete s2;
        }

    } catch (Exception e) {
        throw dicp_solving_error(e.what());
    }
}
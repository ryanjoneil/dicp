//
// Created by Ryan J. O'Neil on 5/8/16.
//

#ifndef DICP_DICPGECODEEXTENSIVESOLVER_H
#define DICP_DICPGECODEEXTENSIVESOLVER_H

#include "DicpSolver.h"

const string SOLVER_GECODE_EXTENSIVE = "gecode-extensive";

class DicpGecodeExtensiveSolver : public DicpSolver {
    void solve(DicpProblem problem);
};


#endif //DICP_DICPGECODEEXTENSIVESOLVER_H

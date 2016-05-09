//
// Created by Ryan J. O'Neil on 5/8/16.
//

#ifndef DICP_DICPSOLVER_H
#define DICP_DICPSOLVER_H

#include <exception>
#include "../DicpProblem.h"

class DicpSolver {
public:
    virtual void solve(DicpProblem problem) = 0;

    static DicpSolver* lookup(string name);
};

class dicp_invalid_solver : public exception { };

class dicp_solving_error : public exception {
public:
    dicp_solving_error(string message) : message{message} { }
    const string message;
};

#endif //DICP_DICPSOLVER_H
//
// Created by Ryan J. O'Neil on 5/6/16.
//

#ifndef DICP_DICPPROBLEM_H
#define DICP_DICPPROBLEM_H

#include <exception>
#include <set>
#include "DicpCommand.h"

using namespace std;

class DicpProblem {
protected:
    set<DicpCommand> commands;
    DicpProblem(set<DicpCommand> commands);

public:
    set <DicpCommand> get_commands(void);
    static DicpProblem load(string filename);
};

class dicp_load_failure : public exception { };

#endif //DICP_DICPPROBLEM_H
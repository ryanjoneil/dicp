//
// Created by Ryan J. O'Neil on 5/7/16.
//

#include <cstdlib>
#include "DicpCommand.h"

using namespace std;

DicpCommand::DicpCommand(string command, int cost) : command(command), cost(cost) { }

bool DicpCommand::operator< (const DicpCommand& cmd) const {
    return command < cmd.command;
}
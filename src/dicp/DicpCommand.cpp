//
// Created by Ryan J. O'Neil on 5/7/16.
//

#include <cstdlib>
#include "DicpCommand.h"

using namespace std;

DicpCommand::DicpCommand(dicp_command_key command, int cost) : command{command}, cost{cost} { }

DicpCommand::DicpCommand(const DicpCommand& command) : DicpCommand{command.command, command.cost} { }

bool DicpCommand::operator< (const DicpCommand& cmd) const {
    return command < cmd.command;
}
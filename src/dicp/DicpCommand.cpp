//
// Created by Ryan J. O'Neil on 5/7/16.
//

#include <cstdlib>
#include "DicpCommand.h"

using namespace std;

int DicpCommand::next_id = 0;

int DicpCommand::get_next_id(void) {
    int id = DicpCommand::next_id;
    DicpCommand::next_id += 1;
    return id;
}

DicpCommand::DicpCommand(dicp_command_key command, int cost) :
        DicpCommand(command, cost, DicpCommand::get_next_id()) { }

DicpCommand::DicpCommand(dicp_command_key command, int cost, int id) :
        command{command}, cost{cost}, id{id} { }

DicpCommand::DicpCommand(const DicpCommand& command) :
        DicpCommand{command.command, command.cost, command.id} { }

DicpCommand& DicpCommand::operator=(const DicpCommand& command) {
    this->command = command.command;
    id = command.id;
    cost = command.cost;
    return *this;
}

bool DicpCommand::operator< (const DicpCommand& cmd) const {
    return command < cmd.command;
}
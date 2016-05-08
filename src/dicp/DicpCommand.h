//
// Created by Ryan J. O'Neil on 5/7/16.
//

#ifndef DICP_DICPCOMMAND_H
#define DICP_DICPCOMMAND_H

#include <iostream>

using namespace std;

typedef string dicp_command_key;

class DicpCommand {
public:
    DicpCommand(dicp_command_key command, int cost);
    DicpCommand(const DicpCommand& command);
    DicpCommand& operator=(const DicpCommand& command);

    const dicp_command_key command;
    const int cost;

    bool operator< (const DicpCommand& cmd) const;
};

#endif //DICP_DICPCOMMAND_H
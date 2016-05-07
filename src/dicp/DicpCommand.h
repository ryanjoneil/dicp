//
// Created by Ryan J. O'Neil on 5/7/16.
//

#ifndef DICP_DICPCOMMAND_H
#define DICP_DICPCOMMAND_H

#include <iostream>

using namespace std;

class DicpCommand {
public:
    DicpCommand(string command, int cost);

    const string command;
    const int cost;

    bool operator< (const DicpCommand& cmd) const;
};

#endif //DICP_DICPCOMMAND_H
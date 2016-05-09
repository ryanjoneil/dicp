//
// Created by Ryan J. O'Neil on 5/7/16.
//

#ifndef DICP_DICPCOMMAND_H
#define DICP_DICPCOMMAND_H

#include <iostream>

using namespace std;

typedef string dicp_command_key;

class DicpCommand {
protected:
    static int next_id;
    static int get_next_id(void);

public:
    DicpCommand(dicp_command_key command, int cost);
    DicpCommand(dicp_command_key command, int cost, int id);
    DicpCommand(const DicpCommand& command);
    DicpCommand& operator=(const DicpCommand& command);

    dicp_command_key command;
    int cost;
    int id;

    bool operator< (const DicpCommand& cmd) const;
};

#endif //DICP_DICPCOMMAND_H
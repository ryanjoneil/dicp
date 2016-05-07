//
// Created by Ryan J. O'Neil on 5/6/16.
//

#include <fstream>
#include "DicpProblem.h"
#include "../third-party/json.hpp"

using json = nlohmann::json;

DicpProblem::DicpProblem(set<DicpCommand> commands) : commands(commands) { }

set<DicpCommand> DicpProblem::get_commands(void) {
    return set<DicpCommand>(commands);
}

DicpProblem DicpProblem::load(string filename) {
    fstream ifs { filename };
    if (!ifs) throw dicp_load_failure();

    set<DicpCommand> cmds { };

    auto input = json::parse(ifs);
    for (json::iterator it = input.begin(); it != input.end(); ++it)
        if (it.key() == "commands")
            for (json::iterator cmdit = (*it).begin(); cmdit != (*it).end(); ++cmdit)
                cmds.insert(DicpCommand(cmdit.key(), cmdit.value()));
        else if (it.key() == "images")
            for (json::iterator imgit = (*it).begin(); imgit != (*it).end(); ++imgit)
                cout << imgit.key() << endl;

    DicpProblem p = DicpProblem(cmds);
    return p;
}
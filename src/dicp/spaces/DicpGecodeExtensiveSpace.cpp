//
// Created by Ryan J. O'Neil on 5/8/16.
//

#include <map>
#include <vector>
#include "DicpGecodeExtensiveSpace.h"

using namespace Gecode;
using namespace std;

DicpGecodeExtensiveSpace::DicpGecodeExtensiveSpace(DicpProblem problem) : problem{problem} {
    schedules = map<dicp_image_key, IntVarArray>{ };
    domains = map<dicp_image_key, IntSet>{ };
    source = BoolVarArray{*this, (int) problem.get_commands().size(), 0, 1};
    total_cost = IntVar(*this, 0, Int::Limits::max);

    // Schedule for each individual image
    vector<DicpImage> images = problem.get_images();
    for (vector<DicpImage>::iterator it = images.begin(); it != images.end(); ++it) {
        // Domains for image schedule slots
        vector<int> command_ids{ };
        vector<DicpCommand> commands = it->get_commands();
        for (vector<DicpCommand>::iterator cit = commands.begin(); cit != commands.end(); ++cit)
            command_ids.push_back(cit->id);

        IntSet domain{&command_ids[0], (int) command_ids.size()};
        pair<dicp_image_key, IntSet> pd{it->image, domain};
        domains.insert(pd);

        // Actual schedule
        IntVarArray a{*this, it->size(), domain};
        pair<dicp_image_key, IntVarArray> p{it->image, a};
        schedules.insert(p);

        // Each command once
        distinct(*this, p.second);

        // First command requires an arc from the source
        for (vector<DicpCommand>::iterator cit = commands.begin(); cit != commands.end(); ++cit)
            rel(*this, source[cit->id] >= expr(*this, a[0] == cit->id));

        // Branching for image schedules
        branch(*this, p.second, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }

    // Schedule cost
    vector<DicpCommand> cmds = problem.get_commands();
    vector<int> costs{ };
    for (vector<DicpCommand>::iterator it = cmds.begin(); it != cmds.end(); ++it)
        costs.push_back(it->cost);

    linear(*this, costs, source, IRT_EQ, total_cost);
}

DicpGecodeExtensiveSpace::DicpGecodeExtensiveSpace(bool share, DicpGecodeExtensiveSpace& s) :
        IntMinimizeSpace(share, s),
        problem{s.problem},
        schedules{s.schedules},
        domains{s.domains},
        source{s.source},
        total_cost{s.total_cost} {

    for (map<dicp_image_key, IntVarArray>::iterator it = s.schedules.begin(); it != s.schedules.end(); ++it)
        schedules.find(it->first)->second.update(*this, share, it->second);
    source.update(*this, share, s.source);
    total_cost.update(*this, share, s.total_cost);
}

Space* DicpGecodeExtensiveSpace::copy(bool share) {
    return new DicpGecodeExtensiveSpace(share, *this);
}

IntVar DicpGecodeExtensiveSpace::cost(void) const {
    return total_cost;
}

void DicpGecodeExtensiveSpace::print(void) const {
    DicpProblem p{problem};
    for (map<dicp_image_key, IntVarArray>::const_iterator it = schedules.begin(); it != schedules.end(); ++it) {
        cout << "[image " << it->first << "] ";
        for (IntVarArray::const_iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
            cout << p.get_command(sit->val()).command << " ";
        cout << endl;
    }
    cout << "[cost] " << total_cost << endl;
    cout << "---------------" << endl;
}
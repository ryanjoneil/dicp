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

        // Branching for image schedules
        branch(*this, p.second, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }
}

DicpGecodeExtensiveSpace::DicpGecodeExtensiveSpace(bool share, DicpGecodeExtensiveSpace& s) :
        Space(share, s), problem{s.problem}, schedules{s.schedules}, domains{s.domains} {
    for (map<dicp_image_key, IntVarArray>::iterator it = s.schedules.begin(); it != s.schedules.end(); ++it)
        schedules.find(it->first)->second.update(*this, share, it->second);
}

Space* DicpGecodeExtensiveSpace::copy(bool share) {
    return new DicpGecodeExtensiveSpace(share, *this);
}

void DicpGecodeExtensiveSpace::print(void) const {
    DicpProblem p{problem};
    for (map<dicp_image_key, IntVarArray>::const_iterator it = schedules.begin(); it != schedules.end(); ++it) {
        cout << "[" << it->first << "] ";
        for (IntVarArray::const_iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
            cout << p.get_command(sit->val()).command << " ";
        cout << endl;
    }
    cout << "---------------" << endl;
}
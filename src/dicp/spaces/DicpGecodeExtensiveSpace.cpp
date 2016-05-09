//
// Created by Ryan J. O'Neil on 5/8/16.
//

#include <map>
#include <vector>
#include "DicpGecodeExtensiveSpace.h"

using namespace Gecode;
using namespace std;

DicpGecodeExtensiveSpace::DicpGecodeExtensiveSpace(DicpProblem problem) {
    schedules = map<dicp_image_key, IntVarArray>{ };

    // Schedule for each individual image
    vector<DicpImage> images = problem.get_images();
    for (vector<DicpImage>::iterator it = images.begin(); it != images.end(); ++it) {
        pair<dicp_image_key, IntVarArray> p{it->image, IntVarArray{*this, 2, 0, 1}};
        schedules.insert(p);
        distinct(*this, p.second);
        branch(*this, p.second, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }
}

DicpGecodeExtensiveSpace::DicpGecodeExtensiveSpace(bool share, DicpGecodeExtensiveSpace& s) : Space(share, s) {
    for (map<dicp_image_key, IntVarArray>::iterator it = s.schedules.begin(); it != s.schedules.end(); ++it) {
        pair<dicp_image_key, IntVarArray> p{it->first, IntVarArray{*this, 2, 0, 1}};
        schedules.insert(p);
        schedules.find(it->first)->second.update(*this, share, it->second);
    }
}

Space* DicpGecodeExtensiveSpace::copy(bool share) {
    return new DicpGecodeExtensiveSpace(share, *this);
}

void DicpGecodeExtensiveSpace::print(void) const {
    for (map<dicp_image_key, IntVarArray>::const_iterator it = schedules.begin(); it != schedules.end(); ++it)
        cout << "[" << it->first << "] " << it->second << endl;
    cout << " ---------------" << endl;
}
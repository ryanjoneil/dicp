//
// Created by Ryan J. O'Neil on 5/8/16.
//

#ifndef DICP_DICPGECODEEXTENSIVESPACE_H
#define DICP_DICPGECODEEXTENSIVESPACE_H

#include <gecode/minimodel.hh>
#include <map>
#include "../DicpImage.h"
#include "../DicpProblem.h"

using namespace Gecode;
using namespace std;

class DicpGecodeExtensiveSpace : public IntMinimizeSpace {
protected:
    DicpProblem problem;
    IntVar zero;
    map<dicp_image_key, IntVarArray> schedules;
    map<dicp_image_key, IntSet> domains;
    BoolVarArray source;
    vector<vector<BoolVarArray>> arcs;
    vector<IntVarArray> nodes;
    vector<vector<IntVarArray>> entries;
    IntVarArray stage_costs;
    IntVar total_cost;

public:
    DicpGecodeExtensiveSpace(DicpProblem problem);

    // Search support
    DicpGecodeExtensiveSpace(bool share, DicpGecodeExtensiveSpace& s);
    virtual Space* copy(bool share);

    // Objective function
    virtual IntVar cost(void) const;

    // Best-solution search
    virtual void constrain(const Space& b);

    // Print solution
    void print(void) const;
};

#endif //DICP_DICPGECODEEXTENSIVESPACE_H

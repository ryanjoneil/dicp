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

class DicpGecodeExtensiveSpace : public Space {
protected:
    DicpProblem problem;
    map<dicp_image_key, IntVarArray> schedules;
    map<dicp_image_key, IntSet> domains;

public:
    DicpGecodeExtensiveSpace(DicpProblem problem);

    // Search support
    DicpGecodeExtensiveSpace(bool share, DicpGecodeExtensiveSpace& s);
    virtual Space* copy(bool share);

    // Print solution
    void print(void) const;
};

#endif //DICP_DICPGECODEEXTENSIVESPACE_H

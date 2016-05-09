//
// Created by Ryan J. O'Neil on 5/7/16.
//

#ifndef DICP_DICPIMAGE_H
#define DICP_DICPIMAGE_H

#include <iostream>
#include <vector>
#include "DicpCommand.h"

using namespace std;

typedef string dicp_image_key;

class DicpImage {
protected:
    vector<DicpCommand> commands;

public:
    DicpImage(dicp_image_key image, vector<DicpCommand> commands);
    DicpImage(dicp_image_key image);
    DicpImage(const DicpImage& image);
    DicpImage& operator=(const DicpImage& image);

    void add_command(DicpCommand cmd);
    vector<DicpCommand> get_commands(void);

    dicp_image_key image;

    bool operator< (const DicpImage& img) const;
};


#endif //DICP_DICPIMAGE_H

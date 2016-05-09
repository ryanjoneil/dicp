//
// Created by Ryan J. O'Neil on 5/6/16.
//

#ifndef DICP_DICPPROBLEM_H
#define DICP_DICPPROBLEM_H

#include <exception>
#include <map>
#include <vector>
#include "DicpCommand.h"
#include "DicpImage.h"

using namespace std;

class DicpProblem {
protected:
    map<dicp_command_key, DicpCommand> commands;
    map<dicp_image_key, DicpImage> images;
    vector<DicpCommand> commands_vec;
    vector<DicpImage> images_vec;
    DicpProblem(map<dicp_command_key, DicpCommand> commands, map<dicp_image_key, DicpImage> images);

public:
    vector <DicpCommand> get_commands(void);
    vector <DicpImage> get_images(void);
    DicpCommand get_command(dicp_command_key command);
    DicpCommand get_command(int id);
    DicpImage get_image(dicp_image_key image);

    static DicpProblem load(string filename);
};

class dicp_load_failure : public exception { };

#endif //DICP_DICPPROBLEM_H
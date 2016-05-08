//
// Created by Ryan J. O'Neil on 5/6/16.
//

#include <fstream>
#include "DicpProblem.h"
#include "../third-party/json.hpp"

using json = nlohmann::json;

DicpProblem::DicpProblem(map<dicp_command_key, DicpCommand> commands, map<dicp_image_key, DicpImage> images) :
        commands(commands), images(images), commands_vec({ }), images_vec({ }) {
    for (map<dicp_command_key, DicpCommand>::iterator it = commands.begin(); it != commands.end(); ++it)
        commands_vec.push_back(it->second);

    for (map<dicp_image_key, DicpImage>::iterator it = images.begin(); it != images.end(); ++it)
        images_vec.push_back(it->second);
}

vector<DicpCommand> DicpProblem::get_commands(void) {
    return vector<DicpCommand>(commands_vec);
}

vector<DicpImage> DicpProblem::get_images(void) {
    return vector<DicpImage>(images_vec);
}

DicpCommand DicpProblem::get_command(dicp_command_key command) {
    return commands.find(command)->second;
}

DicpImage DicpProblem::get_image(dicp_image_key image) {
    return images.find(image)->second;
}

DicpProblem DicpProblem::load(string filename) {
    fstream ifs { filename };
    if (!ifs) throw dicp_load_failure { };

    map<dicp_command_key, DicpCommand> cmds { };
    map<dicp_image_key, DicpImage> imgs { };

    auto input = json::parse(ifs);
    for (json::iterator it = input.begin(); it != input.end(); ++it)
        if (it.key() == "commands")
            for (json::iterator cmdit = (*it).begin(); cmdit != (*it).end(); ++cmdit) {
                pair<dicp_command_key, DicpCommand> p {cmdit.key(), DicpCommand{cmdit.key(), cmdit.value()}};
                cmds.insert(p);
            }

        else if (it.key() == "images")
            for (json::iterator imgit = (*it).begin(); imgit != (*it).end(); ++imgit) {
                DicpImage i {imgit.key()};
                pair<dicp_image_key, DicpImage> p {i.image, i};
                imgs.insert(p);

                for (json::iterator imgcmdit = (*imgit).begin(); imgcmdit != (*imgit).end(); ++imgcmdit)
                     imgs.find(i.image)->second.add_command(cmds.find(imgcmdit.value())->second);
            }

    DicpProblem p = DicpProblem(cmds, imgs);
    return p;
}
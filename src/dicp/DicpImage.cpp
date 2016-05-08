//
// Created by Ryan J. O'Neil on 5/7/16.
//

#include <cstdlib>
#include "DicpImage.h"

using namespace std;

DicpImage::DicpImage(dicp_image_key image, vector<DicpCommand> commands) : image{image}, commands{commands} { }

DicpImage::DicpImage(dicp_image_key image) : DicpImage{image, {}} { }

DicpImage::DicpImage(const DicpImage& image) : DicpImage{image.image, image.commands} { }

void DicpImage::add_command(DicpCommand cmd) {
    commands.push_back(cmd);
}

vector<DicpCommand> DicpImage::get_commands(void) {
    return vector<DicpCommand>{ commands };
}

bool DicpImage::operator< (const DicpImage& img) const {
    return image < img.image;
}
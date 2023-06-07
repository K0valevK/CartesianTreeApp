#ifndef ACTIONBUTTONS_H
#define ACTIONBUTTONS_H

#include <array>
#include <string>

namespace GraphicProject {

enum Actions {
    CreateNodeRandom, CreateNodeKey, CreateNodeCustom, Merge, Split, Insert, Erase
};

static const std::array<std::string, 7> action_names = {"", "", "", "Merge", "Split", "Insert", "Erase"};

} // namespace GraphicProject

#endif // ACTIONBUTTONS_H

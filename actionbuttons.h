#ifndef ACTIONBUTTONS_H
#define ACTIONBUTTONS_H

#include <array>
#include <string>

namespace GraphicProject {

enum Actions {
    Merge, Split, Insert, Erase
};

static const std::array<std::string, 4> action_names = {"Merge", "Split", "Insert", "Erase"};

} // namespace GraphicProject

#endif // ACTIONBUTTONS_H

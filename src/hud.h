// Based on: https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/hud.h
// (This file, like many files within that repository, is based
// on code within https://github.com/j-dax/gd-cpp. That project
// is copyrighted by Matthew Piazza and released under the BSD 3-Clause license

#pragma once

#include <godot_cpp/classes/canvas_layer.hpp>

using namespace godot;

class Hud : public CanvasLayer {
    GDCLASS(Hud, CanvasLayer)

private:
    bool reset;
    Size2 screenSize;

    static void _bind_methods();

public:
    int players_to_include = 2;
    Hud();
    ~Hud() = default;
    void show_message(String text);
    void _process(double delta) override;
    // Note: A compiler error helped me recognize that, unless
    // you include the double delta parameter,
    // this function won't actually override the default
    // _process function.
    // Based on
    // https://docs.godotengine.org/en/stable/tutorials/scripting/idle_and_physics_processing.html
    // and mnchar.h
};
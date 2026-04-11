// Based on: https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/hud.h
// (This file, like many files within that repository, is based
// on code within https://github.com/j-dax/gd-cpp. That project
// is copyrighted by Matthew Piazza and released under the BSD 3-Clause license

#pragma once

#include <godot_cpp/classes/canvas_layer.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class Hud : public CanvasLayer {
    GDCLASS(Hud, CanvasLayer)

private:
    bool reset;
    Size2 screenSize;

    static void _bind_methods();

public:
    bool can_launch_new_game = true;
    double message_time = 0.0;
    double reset_overall_stats_time = 0.0;
    bool announce_winner = false;
    Dictionary players_to_include;

    // Creating a dictionary that can keep track of how long each 
    // player has continuously held down the reset button. (This will
    // allow us to determine whether or not to reset the game's
    // overall stats.)
    TypedDictionary<String, double> id_reset_time_dict {{"0", 0.0},
    {"1", 0.0},
    {"2", 0.0},
    {"3", 0.0},
    {"4", 0.0},
    {"5", 0.0},
    {"6", 0.0}};

// The following dictionary maps each ID to a name for its corresponding
// color (defined within mnchar_id_color_dict). That will help
// players understand which Mnchar a given player ID refers to.
TypedDictionary<String, String> mnchar_id_color_name_dict {
{String("0"), "Blue"},
{String("1"), "Green"},
{String("2"), "Cyan"},
{String("3"), "Red"},
{String("4"), "Magenta"},
{String("5"), "Yellow"},
{String("6"), "White"},
{String("7"), "Black"}
};


    Hud();
    ~Hud() = default;
    String winner_message {""};
    String constant_message {""};
    void show_message(String text);
    void show_constant_message(String text);
    void _process(double delta) override;
    // Note: A compiler error helped me recognize that, unless
    // you include the double delta parameter,
    // this function won't actually override the default
    // _process function.
    // Based on
    // https://docs.godotengine.org/en/stable/tutorials/scripting/idle_and_physics_processing.html
    // and mnchar.h
};
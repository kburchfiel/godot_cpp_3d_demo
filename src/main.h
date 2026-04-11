// Based in part on
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.h
// and on mnchar.h within this project

#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>
#include <godot_cpp/templates/hash_set.hpp>
#include <godot_cpp/core/math_defs.hpp>
#include <cmath>

using namespace godot;

class Main : public Node {
  GDCLASS(Main, Node)

private:
  Ref<PackedScene> mnchar_scene;

protected:
  static void _bind_methods();

public:
  Main();
  ~Main();

  Ref<PackedScene> get_mnchar_scene();
  void set_mnchar_scene(Ref<PackedScene>);

  void _ready();

  void _on_hud_start_game(Dictionary players_to_include);

  void _on_hud_reset_overall_stats();

  void _on_mnchar_mnchar_hit(String hit_mnchar_id_arg, 
  String firing_mnchar_id_arg);

  void end_game(String winning_mnchar_id);

  int number_of_players = 0;


// The following TypedDictionary code was based on
// https://docs.godotengine.org/en/stable/engine_details/architecture/core_types.html ,  
// https://github.com/godotengine/godot-cpp/blob/master/test/src/example.cpp ,
// and my understanding of std::map initialization options.

  TypedDictionary<String, Vector3> mnchar_id_location_dict {
{String("0"), Vector3(15, 0, -20)},
{String("1"), Vector3(-15, 0, 20)},
{String("2"), Vector3(-20, 0, -15)},
{String("3"), Vector3(20, 0, 15)},
{String("4"), Vector3(-5, 0, -20)},
{String("5"), Vector3(5, 0, 20)},
{String("6"), Vector3(-20, 0, 5)},
{String("7"), Vector3(20, 0, -5)}
};

// The following dictionary specifies the starting rotation of 
// each character. These rotations allow each character to face
// towards the center of the game area. 
// I initally multiplied each value by
// the quotient of Math_PI (which I found in math_defs.hpp) and
// 180 converts degree arguments into radians. However, I then
// realized that it would be possible to simplify these entries
// further.

TypedDictionary<String, double> mnchar_id_rotation_dict {
{String("0"), 0},
{String("1"), Math_PI},
{String("2"), Math_PI/2},
{String("3"), Math_PI/-2},
{String("4"), 0},
{String("5"), Math_PI},
{String("6"), Math_PI/2},
{String("7"), Math_PI/-2},
};


// For the color dict, I simply treated colors as 3-bit binary
// numbers, then incremented them up by one bit (except that 
// I started at (0, 0, 1) rather than (0, 0, 0).

  TypedDictionary<String, Color> mnchar_id_color_dict {
{String("0"), Color(0, 0, 1, 1)},
{String("1"), Color(0, 1, 0, 1)},
{String("2"), Color(0, 1, 1, 1)},
{String("3"), Color(1, 0, 0, 1)},
{String("4"), Color(1, 0, 1, 1)},
{String("5"), Color(1, 1, 0, 1)},
{String("6"), Color(1, 1, 1, 1)},
{String("7"), Color(0, 0, 0, 1)}
};

// Note: You can also add values to typed dictionaries using
// the [] operator, but this must be done within a function
// (to the best of my knowledge). Example:

// TypedDictionary<String, Vector3> gen_id_location_tdict() {

// TypedDictionary<String, Vector3> id_location_tdict;

// // Note: The following code will *not* work outside of a function
// // (at least based on my tests).
// id_location_tdict[String("0")] = Vector3(20, 0, -20);
// id_location_tdict[String("1")] = Vector3(-20, 0, -20);

// return id_location_tdict;

// }

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

// The following HashSet will keep a list of active players.
// Once the size of this set becomes less than two, we can
// determine which player (if any) won the game.
// (If two players hit each other at exactly the same time,
// the length of this set will become 0, and no winner will be
// declared.)

// See https://docs.godotengine.org/en/stable/engine_details/architecture/core_types.html ,
// and https://github.com/godotengine/godot/blob/master/core/templates/hash_set.h ,
// and godot-cpp/include/godot_cpp/templates/hash_set.hpp 

HashSet<String> active_players {};


// Creating another TypedDictionary that will keep track of how many
// hits each player has achieved so far:
TypedDictionary<String, int> hits_achieved {};

// Creating two additional dictionaries that will store total
// hit and win counts across games:
TypedDictionary<String, int> overall_hits_achieved {};


TypedDictionary<String, int> overall_wins {};

void update_constant_message();

};
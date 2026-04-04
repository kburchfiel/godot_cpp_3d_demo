// Based in part on
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.h
// and on mnchar.h within this project

#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>
#include <godot_cpp/templates/hash_set.hpp>

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

  void _on_hud_start_game(int players_to_include);

  void _on_mnchar_mnchar_hit(String hit_mnchar_id_arg);

  void end_game(String winning_mnchar_id);


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

// To do: Add in an ID-rotation dict so that players are all facing
// towards the center of the game area. Pass this rotation value
// to Mnchar.cpp's start() function also.

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


};
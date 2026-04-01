// Based in part on:
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp
// and on my mnchar.cpp code

#include "main.h"
#include "mnchar.h"
#include "hud.h"
#include <godot_cpp/classes/scene_tree.hpp>

void Main::_bind_methods() {

  ClassDB::bind_method(D_METHOD("get_mnchar_scene"), &Main::get_mnchar_scene);
  ClassDB::bind_method(D_METHOD("set_mnchar_scene", "mnchar_scene"),
                       &Main::set_mnchar_scene);
  ClassDB::bind_method(D_METHOD("_on_hud_start_game"),
                       &Main::_on_hud_start_game);
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packed_scene",
                            PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
               "set_mnchar_scene", "get_mnchar_scene");
}

Ref<PackedScene> Main::get_mnchar_scene() { return mnchar_scene; }

void Main::set_mnchar_scene(Ref<PackedScene> packed_scene) {
  mnchar_scene = packed_scene;
}

Main::Main() {}

Main::~Main() {}

void Main::_ready() {

}

void Main:: _on_hud_start_game()
{
  // Determining how many players to add:
  // (Note: This could probably also be retrieved by emitting
  // a player-count argument along with the "start_game" signal.)
  int players_to_add = get_node<Hud>("Hud")->players_to_include;
  UtilityFunctions::print(players_to_add);

// Adding as many players to our scene as specified by players_to_add
// (while also specifying unique IDs for them)
  for (int i = 0; i < players_to_add; i++)
  // Remember that valid player IDs will range from 0 to 7--so,
// for a 2-player game, we'll want i to iterate from 0 to 1 
// (inclusive).
{String new_player_id = String::num(i).substr(0, 1);
  // substr() should convert "3.0" to "3", which will be useful
  // for accurately setting IDs.

  // At least two players will always be added.
  auto new_mnchar = reinterpret_cast<Mnchar *>(
  get_mnchar_scene()->instantiate());
  new_mnchar->start(new_player_id);
  add_child(new_mnchar);


}



  // auto mnchar_1 = reinterpret_cast<Mnchar *>(mnchar_scene->instantiate());
  // mnchar_1->start("1");

  // add_child(mnchar_1);

  
  // auto mnchar_1 = reinterpret_cast<Mnchar *>(mnchar_scene->instantiate());
  // mnchar_1->start("1");

  // add_child(mnchar_1);

  //   auto mnchar_1 = reinterpret_cast<Mnchar *>(mnchar_scene->instantiate());
  // mnchar_1->start("1");

  // add_child(mnchar_1);

//   UtilityFunctions::print("At the end of Main::_ready, \
// the mnchar_id values of mnchar_0 and mnchar_1 are \
// ",
//                           mnchar_0->get_mnchar_id(), " and ",
//                           mnchar_1->get_mnchar_id());
}



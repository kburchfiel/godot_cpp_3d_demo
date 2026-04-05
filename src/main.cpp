// Based in part on:
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp
// and on my mnchar.cpp code

#include "main.h"
#include "mnchar.h"
#include "hud.h"
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/signal.hpp>

void Main::_bind_methods() {

  ClassDB::bind_method(D_METHOD("get_mnchar_scene"), &Main::get_mnchar_scene);
  ClassDB::bind_method(D_METHOD("set_mnchar_scene", "mnchar_scene"),
                       &Main::set_mnchar_scene);
  ClassDB::bind_method(D_METHOD("_on_hud_start_game"),
                       &Main::_on_hud_start_game);
  
  ClassDB::bind_method(D_METHOD(
  "_on_mnchar_mnchar_hit", "hit_mnchar_id_arg"), &Main::_on_mnchar_mnchar_hit);

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


void Main::end_game(String winning_mnchar_id) {
// At this point, we'll want to clear out all remaining players
// (though only one should be present); inform the players who won;
// and then set the Hud's can_launch_new_game boolean to true
// so that players can launch a new game if desired.
get_tree()->call_group("mnchars", "queue_free"); // This code, 
// based on the game_over() function within
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp ,
// requires that you create a 'mnchars' group within the editor
// so that it can be referenced by this function. See
// the 'Removing old creeps' section of 
// https://docs.godotengine.org/en/4.5/getting_started/first_2d_game/06.heads_up_display.html
// for more details.

// Specifying which player won: (winner_message starts out as blank.
// The double line breaks at the end will help distinguish the winner
// from the instructions.

String new_winner_message = "The winning player is: "+winning_mnchar_id + "\n\n";


// Reporting how many hits each player scored received:

  for (int i = 0; i < number_of_players; i++)
  // Remember that valid player IDs will range from 0 to 7--so,
// for a 2-player game, we'll want i to iterate from 0 to 1 
// (inclusive).
{String mnchar_id_arg = String::num(i).substr(0, 1);
new_winner_message += "Player " + mnchar_id_arg 
+ " scored " + String::num(hits_achieved[mnchar_id_arg]) 
+ " hits.\n";
}

// Creating some space between this message and information
// that will follow:
new_winner_message += "\n\n";



get_node<Hud>("Hud")->winner_message = new_winner_message;
get_node<Hud>("Hud")->message_time = 0; // Restarting the message
// timer that will get utilized within Hud's _process() function
get_node<Hud>("Hud")->can_launch_new_game = true;


}


// Don't forget to preface the function definitions in this file 
// with Main:: !
void Main::_on_mnchar_mnchar_hit(String hit_mnchar_id_arg,
String firing_mnchar_id_arg) {
  UtilityFunctions::print(
"Mnchar with an ID of ", hit_mnchar_id_arg, "was just hit by Mnchar \
with an ID of ", firing_mnchar_id_arg);
active_players.erase(hit_mnchar_id_arg);
UtilityFunctions::print("Current size of active_players:", active_players.size());

// Registering this hit within our hits_achieved dictionary:
int current_hit_value = hits_achieved[firing_mnchar_id_arg];
current_hit_value += 1;
hits_achieved[firing_mnchar_id_arg] = current_hit_value;

if (active_players.size() == 1)

{
// Checking to see who won: (We'll do so by iterating through all
// 8 player IDs).
for (int i = 0; i < 8; i++) 
{String mnchar_to_look_up = String::num(i).substr(0, 1);
UtilityFunctions::print(i);
// Checking whether the output of find() matches that of (end). If 
// it *doesn't*, we can conclude that this element is present 
// within the vector, and thus the winner.
// Based on:
// https://www.geeksforgeeks.org/cpp/how-to-check-if-set-contains-an-element-in-cpp/
if (active_players.find(mnchar_to_look_up) != active_players.end())

{end_game (mnchar_to_look_up);
break;}

}

if (active_players.size() == 0) // In this case, no one won the game.
{
end_game("Nobody"); // 
}

}

}

void Main::_ready() {

}




void Main:: _on_hud_start_game(int players_to_include)
{

number_of_players = players_to_include; // number_of_players
// will be useful for iterating through dictionaries that contain
// player information

// Resetting our set of active players:
active_players.clear();

// Resetting our hits dictionary also: (This may be necessary
// if a new game includes fewer players than a previous game)
hits_achieved = TypedDictionary<String, int>{};

  // Determining how many players to add:
  // (Note: This could probably also be retrieved by emitting
  // a player-count argument along with the "start_game" signal.)
  // int players_to_include = get_node<Hud>("Hud")->players_to_include;
  UtilityFunctions::print("Received the following players_to_include \
argument from the signal: ", players_to_include);

// Adding as many players to our scene as specified by players_to_include
// (while also specifying unique IDs for them)
  for (int i = 0; i < players_to_include; i++)
  // Remember that valid player IDs will range from 0 to 7--so,
// for a 2-player game, we'll want i to iterate from 0 to 1 
// (inclusive).
{String mnchar_id_arg = String::num(i).substr(0, 1);
  // substr() should convert "3.0" to "3", which will be useful
  // for accurately setting IDs.

  // Determining the colors and translate values that correspond
  // to this ID:
  Color mnchar_color_arg = mnchar_id_color_dict[mnchar_id_arg];
  Vector3 mnchar_translate_arg = mnchar_id_location_dict[mnchar_id_arg];

  // Adding a new mnchar to the scene:
  auto new_mnchar = reinterpret_cast<Mnchar *>(
  get_mnchar_scene()->instantiate());



// Connecting this mnchar's "mnchar_hit" signal to Main: 
// on_mnchar_mnchar_hit():
// (I couldn't do this within the editor, since at the outset of 
// the game, a Mnchar is not present within the scene tree. Therefore,
// I used this alternative approach (which also has the benefit
// of not requiring any editor-specific updates.)
// https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html#signals

  new_mnchar->connect("mnchar_hit", Callable(this, "_on_mnchar_mnchar_hit"));

  // Note that it doesn't seem necessary to mention 
  // the mnchar_id or firing_mnchar_id propeorties of mnchar_hit 
  // within this code, as it will still successfully get picked up by 
  // the _on_mnchar_mnchar_hit function specified within Callable.



  new_mnchar->start(mnchar_id_arg, mnchar_color_arg, 
  mnchar_translate_arg);
  add_child(new_mnchar);


  // Adding this player to our set of active players:

  active_players.insert(mnchar_id_arg);
  hits_achieved[mnchar_id_arg] = 0;

  
  

}

UtilityFunctions::print("Current contents of active_players:");

for (int i = 0; i < 8; i++) // For many use cases, you could limit
// i to active_players.size()
{String mnchar_to_look_up = String::num(i).substr(0, 1);
UtilityFunctions::print(i);
// Checking whether the output of find() matches that of (end). If 
// it *doesn't*, we can conclude that this element is present 
// within the vector.
// Based on:
// https://www.geeksforgeeks.org/cpp/how-to-check-if-set-contains-an-element-in-cpp/
UtilityFunctions::print(active_players.find(mnchar_to_look_up) != active_players.end());
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



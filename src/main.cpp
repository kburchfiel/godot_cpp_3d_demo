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
  

// In order to successfully connect other nodes' signals to
// functions within main.cpp, we need to make sure to bind the
// relevant main.cpp functions here.              
  ClassDB::bind_method(D_METHOD(
  "_on_mnchar_mnchar_hit", "hit_mnchar_id_arg"), &Main::_on_mnchar_mnchar_hit);

ClassDB::bind_method(D_METHOD(
  "_on_hud_reset_overall_stats"), &Main::_on_hud_reset_overall_stats);

ClassDB::bind_method(D_METHOD(
  "_on_mnchar_reset_game"), &Main::_on_mnchar_reset_game);


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

// Note: explicitly converting the output of 
// mnchar_id_color_name_dict[winning_mnchar_id] to a String
// prevents a compiler error related to an ambiguous conversion. 
String new_winner_message = "The winning player \
is: "+winning_mnchar_id;

if (winning_mnchar_id != "Nobody")
{
new_winner_message += " (" + 
String(mnchar_id_color_name_dict[
winning_mnchar_id]) + ")";
}

new_winner_message += "\n\n";



// Reporting how many hits each player scored received:

Array hits_achieved_keys = hits_achieved.keys();

for (int key_index = 0; 
key_index < hits_achieved_keys.size(); key_index++)

{
// Determining how many hits each player achieved in this game:
String mnchar_id_arg = String(hits_achieved_keys[key_index]);
String current_id_hits_achieved = String::num_int64(
hits_achieved[mnchar_id_arg]);

new_winner_message += "Player " + mnchar_id_arg + " (" + 
String(mnchar_id_color_name_dict[mnchar_id_arg]) + ")"
+  " scored " + current_id_hits_achieved + " hits.\n";
}

// String::num_int64 converts a string into an integer. Using
// String::num will instead convert it into a float, which
// is problematic for cases in which 
// the output must equal an integer. 

// Creating some space between this message and information
// that will follow:
new_winner_message += "\n\n";


get_node<Hud>("Hud")->winner_message = new_winner_message;
get_node<Hud>("Hud")->message_time = 0; // Restarting the message
// timer that will get utilized within Hud's _process() function

// Clearing out the Hud class's players_to_include dictionary so that
// a new set of players can be added:
get_node<Hud>("Hud")->players_to_include = Dictionary {};
get_node<Hud>("Hud")->can_launch_new_game = true;


}


void Main::_on_mnchar_reset_game()
{

// This function allows us to move back to the player-selection
// menu and to remove hits scored within the current game from
// the overall-hits dictionary.

Array hits_achieved_keys = hits_achieved.keys();

for (int key_index = 0; 
key_index < hits_achieved_keys.size(); key_index++)

{
// Determining how many hits each player achieved in this game:
String current_id = String(hits_achieved_keys[key_index]);
int current_id_hits_achieved = hits_achieved[current_id];

// Removing these hits from the overall-hits dictionary:
// (The player's ID will still be present in the dictionary,
// but this shouldn't be an issue.)
overall_hits_achieved[current_id] = int(
overall_hits_achieved[current_id]) - current_id_hits_achieved;
}

// Updating the overall_hits_achieved dictionary present within
// the HUD:
update_constant_message();


end_game("Nobody");
}


// Don't forget to preface the function definitions in this file 
// with Main:: !
void Main::_on_mnchar_mnchar_hit(String hit_mnchar_id_arg,
String firing_mnchar_id_arg) {
  UtilityFunctions::print(
"Mnchar with an ID of ", hit_mnchar_id_arg, " was just hit by Mnchar \
with an ID of ", firing_mnchar_id_arg);
active_players.erase(hit_mnchar_id_arg);
UtilityFunctions::print("Current size of active_players:", active_players.size());

// Registering this hit within our hits_achieved dictionary:
int current_hit_value = hits_achieved[firing_mnchar_id_arg];
current_hit_value += 1;
hits_achieved[firing_mnchar_id_arg] = current_hit_value;

// Updating overall_hits_achieved as well:
int current_overall_hit_value = overall_hits_achieved[
firing_mnchar_id_arg];
current_overall_hit_value += 1;
overall_hits_achieved[firing_mnchar_id_arg] = current_overall_hit_value;

// UtilityFunctions::print("Relevant key and value:", 
//   firing_mnchar_id_arg, overall_hits_achieved[firing_mnchar_id_arg]);

// Updating our overall stats to reflect this new hit:
update_constant_message();

if (active_players.size() == 1)

{
// Checking to see who won:
// Since there's only one character in this set, we can identify
// the winning character by creating an iterator to the first
// (and only) item in this set (using .begin()),
// then dereferencing it to identify the winning ID.
// For reference, see:
// https://stackoverflow.com/a/12863273/13097194
String winning_mnchar = *active_players.begin();




// for (int i = 0; i < 8; i++) 
// {String mnchar_to_look_up = String::num_int64(i);
// UtilityFunctions::print(i);
// // Checking whether the output of find() matches that of (end). If 
// // it *doesn't*, we can conclude that this element is present 
// // within the vector, and thus the winner.
// // (I had learned about the has() function for arrays,
// // but was pleased to find out that it's also available for
// // sets. See
// // godot-cpp/include/godot_cpp/templates/hash_set.hpp )

// if (active_players.has(mnchar_to_look_up))


// // Alternative approach, based on 
// // https://www.geeksforgeeks.org/cpp/how-to-check-if-set-contains-an-element-in-cpp/ :
// // if (active_players.find(mnchar_to_look_up) != active_players.end())

int current_overall_win_value = overall_wins[
winning_mnchar];
current_overall_win_value += 1;
overall_wins[winning_mnchar] = current_overall_win_value;

// Updating our overall stats to reflect this new hit:
update_constant_message();
  
  
  end_game (winning_mnchar);


}

else if (active_players.size() == 0) // In this case, no one won the game.
{
end_game("Nobody"); // 
}

}

void Main::_ready() {
// Connecting the HUD's overall-stats-reset and start-game signals
// to corresponding functions within Main:
// (These connections could also be done within the editor, but
// I wanted to demonstrate that it's possible to do so via code
// as well. In addition, I found that these connected signals
// would sometimes disappear from the editor--so connecting them
// here saved me the trouble of periodically reconnecting them.)
get_node<Hud>("Hud")->connect(
  "reset_overall_stats", Callable(this, "_on_hud_reset_overall_stats"));
get_node<Hud>("Hud")->connect(
  "start_game", Callable(this, "_on_hud_start_game"));

}

void Main::_on_hud_reset_overall_stats()
// This function resets our overall-stats dictionaries (which can 
// be helpful, for instance, following a practice run or some
// other issue).
{
  UtilityFunctions::print("_on_hud_reset_overall_stats just got called.");
  overall_hits_achieved = TypedDictionary<String, int>{};
  overall_wins = TypedDictionary<String, int>{};

  // Replacing our existing overall-stats dictionaries with
  // these new, blank ones:
  update_constant_message();
}


void Main::update_constant_message()
/* This function prints out the contents of two dictionaries--one
containing players' overall (i.e. game-to-game) hit counts,
and another containing their overall wins. It should be called 
whenever one of these two dictionaries is changed.
*/
{
UtilityFunctions::print("Calling update_constant_message.");

// We could also add information about hits for the current game
// (see below), but since these will already get shared
// within the end-of-game text, I commented out this section.

// String constant_message_text = "Current-game hits:\n";

// Array hits_achieved_keys = hits_achieved.keys();

// // for (int array_index = 0; array_index < hits_achieved_keys.size(); array_index ++)
// // UtilityFunctions::print("Value with an array index of ", 
// // array_index, "is:", hits_achieved_keys[array_index]);

// for (int key_index = 0; 
// key_index < hits_achieved_keys.size(); key_index++)

// {constant_message_text += "Player "+ String(
// hits_achieved_keys[key_index]) + ": " +
// String::num(hits_achieved[hits_achieved_keys[key_index]]) + "\n";}


String constant_message_text = "Overall hits:\n";

// Iterating through a TypedDictionary by
// accessing its keys, then printing out each key
// and its corresponding value pair:
// Based on the answer by svrcrz at
// https://godotforums.org/d/33822-how-to-loop-a-dictionary-in-godot-c-gdextension/3

Array overall_hits_achieved_keys = overall_hits_achieved.keys();

// for (int array_index = 0; array_index < overall_hits_achieved_keys.size(); array_index ++)
// UtilityFunctions::print("Value with an array index of ", 
// array_index, "is:", overall_hits_achieved_keys[array_index]);


for (int key_index = 0; 
key_index < overall_hits_achieved_keys.size(); key_index++)

{constant_message_text += "Player "+ String(
overall_hits_achieved_keys[key_index]) + " (" + 
String(mnchar_id_color_name_dict[String(
overall_hits_achieved_keys[key_index])]) + "): " +
String::num_int64(overall_hits_achieved[
  overall_hits_achieved_keys[key_index]]) + "\n";}

Array overall_wins_keys = overall_wins.keys();

constant_message_text += "\nOverall wins:\n";

for (int key_index = 0; 
key_index < overall_wins_keys.size(); key_index++)

{constant_message_text+=
"Player "+ String(overall_wins_keys[key_index]) + " (" + 
String(mnchar_id_color_name_dict[String(
overall_hits_achieved_keys[key_index])]) + "): " +
String::num_int64(overall_wins[overall_wins_keys[key_index]]) + "\n";}

// Updating the ConstantMessage label within our Hud class to
// feature this new message:

get_node<Hud>("Hud")->show_constant_message(constant_message_text);

}


void Main:: _on_hud_start_game(Dictionary players_to_include)
{

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

// The following loop is based on
// https://www.geeksforgeeks.org/cpp/different-ways-to-iterate-over-a-set-in-c/ .
// See also:
// https://stackoverflow.com/questions/12863256/how-to-iterate-stdset

Array players_to_include_keys = players_to_include.keys();

for (int key_index = 0; 
key_index < players_to_include_keys.size(); key_index++)
{
String mnchar_id_arg = players_to_include_keys[key_index];

  // Determining the colors and translate values that correspond
  // to this ID:
  Color mnchar_color_arg = mnchar_id_color_dict[mnchar_id_arg];
  Vector3 mnchar_translate_arg = mnchar_id_location_dict[mnchar_id_arg];
  double mnchar_rotation_arg = mnchar_id_rotation_dict[mnchar_id_arg];

  // Adding a new mnchar to the scene:
  auto new_mnchar = reinterpret_cast<Mnchar *>(
  get_mnchar_scene()->instantiate());



// Connecting this Mnchar instance's "mnchar_hit" signal to Main: 
// on_mnchar_mnchar_hit():
// (I couldn't do this within the editor, since at the outset of 
// the game, a Mnchar is not present within the scene tree. Therefore,
// I used this alternative approach (which also has the benefit
// of not requiring any editor-specific updates.)
// https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html#signals

  new_mnchar->connect("mnchar_hit", Callable(this, "_on_mnchar_mnchar_hit"));


  // Note that it doesn't seem necessary to mention 
  // the mnchar_id or firing_mnchar_id properties of mnchar_hit 
  // within this code, as it will still successfully get picked up by 
  // the _on_mnchar_mnchar_hit function specified within Callable.

// Performing a similar operation for the Mnchar class's 
// reset_game signal:

new_mnchar->connect("reset_game", Callable(this, "_on_mnchar_reset_game"));


  new_mnchar->start(mnchar_id_arg, mnchar_color_arg, 
  mnchar_translate_arg, mnchar_rotation_arg);
  add_child(new_mnchar);


  // Adding this player to our set of active players:

  active_players.insert(mnchar_id_arg);
  hits_achieved[mnchar_id_arg] = 0;

  // Checking to see whether this Mnchar ID is present within 
  // our dictionaries of overall hits and wins. If it's not,
  // we'll add it to that dictionary, then make 0 its starting
  // value.


    if (overall_hits_achieved.has(mnchar_id_arg) == false)
  // Alternative option:
  // if (overall_hits_achieved_keys.has(mnchar_id_arg) == false)
  // Based on https://docs.godotengine.org/en/stable/classes/class_array.html#class-array-method-find
  {  overall_hits_achieved[mnchar_id_arg] = 0;}

    if (overall_wins.has(mnchar_id_arg) == false)
  {  overall_wins[mnchar_id_arg] = 0;}


  
  

}

UtilityFunctions::print("Current contents of active_players:");

for (int i = 0; i < 8; i++) // For many use cases, you could limit
// i to active_players.size()
{String mnchar_to_look_up = String::num_int64(i);
UtilityFunctions::print(i);
// Checking whether the output of find() matches that of (end). If 
// it *doesn't*, we can conclude that this element is present 
// within the vector.
// Based on:
// https://www.geeksforgeeks.org/cpp/how-to-check-if-set-contains-an-element-in-cpp/
// UtilityFunctions::print(active_players.find(mnchar_to_look_up) != active_players.end());

// Simpler approach that I'm now using in place of the commented-out
// one above:
UtilityFunctions::print(active_players.has(mnchar_to_look_up));
}

//HashSet<String>::Iterator active_players_iterator = active_players.begin();


// Note that you need to place the ++ operator before, rather than
// after, active_players_iterator.
UtilityFunctions::print("Printing out all active players in set:");
for (auto active_players_iterator = active_players.begin();
active_players_iterator != active_players.end(); ++active_players_iterator)
// See: https://stackoverflow.com/a/12863273/13097194
{
UtilityFunctions::print(*active_players_iterator);
}


// HashSet<String>::Iterator active_players_iterator = active_players.begin();

// UtilityFunctions::print("Printing out all active players in set:");
// while (active_players_iterator != active_players.end())
// {
// UtilityFunctions::print(*active_players_iterator);
// ++active_players_iterator;
// }


update_constant_message();


}



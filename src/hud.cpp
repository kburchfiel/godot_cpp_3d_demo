// Based on https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/hud.cpp


#include "hud.h"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/variant/signal.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include "main.h"

using namespace godot;

Hud::Hud() { reset = false; }

void Hud::_bind_methods() {
ADD_SIGNAL(MethodInfo("start_game", PropertyInfo(
Variant::DICTIONARY, "players_to_include")));

ADD_SIGNAL(MethodInfo("reset_overall_stats"));
// For string-based arguments, use VARIANT::STRING instead.
// Note that PropertyInfo is included within the MethodInfo
// parentheses.
// Source: https://github.com/godotengine/godot-cpp/blob/master/test/src/example.cpp

}

void Hud::show_message(String text) {
  auto message = get_node<Label>("Message");
  message->set_text(text);
  message->show();
}

void Hud::show_constant_message(String text) {
  auto message = get_node<Label>("ConstantMessage");
  message->set_text(text);
  message->show();
}

void Hud::_process(double delta) {

if (can_launch_new_game == true) // We'll only want to enable
// the following functionality if a game isn't aready in progress.
{

// The following input-parsing code is based on mnchar.cpp.

    message_time += delta;

    auto input = Input::get_singleton();

    // The following boolean will get set to true if any connected
    // devices are holding down the reset button. This allows the 
    // game to be reset as long as at least one reset button
    // is being held down for at least 2 seconds.
    bool anyone_holding_reset_button = false;

    if (message_time >= 2.0)
{
    for (int i = 0; i < 8; i++)

    {

    String strint = String::num_int64(i); // 'strint' is short
    // for 'string version of int' :)

    // Players can add themselves to the game by clicking the Fire
    // button on their controllers. (I had previously specified
    // the number of players by having the first player choose
    // a number between 2 and 8; however, this setup is less flexible,
    // since players may want to sit some games out and then come
    // back later. Therefore, having each player specify that
    // he/she wants to be in this particular game is helpful.)

    if ((input->is_action_just_pressed("fire_"+strint)) &&
    (announce_winner == false)) 
    // Adding a player ID that corresponds to this device ID to
    // players_to_include:

    // (Checking for announce_winner prevents players from 
    // accidentally adding themselves into this new game in the
    // process of attempting to fire a projectile within a game
    // that just ended.)

    // Note: an alternative approach would have been to add these
    // IDs to a HashSet. However, I don't think it's possible to
    // pass HashSets as arguments within signals, as they're
    // not parts of the Variant class; see
    // https://docs.godotengine.org/en/stable/engine_details/architecture/core_types.html .
    // Therefore, I used a dictionary for this task instead;
    // the keys will store IDs, and the values will simply be zeros
    // (with no meaning).
    // I also tried using a TypedDictionary initially, but since
    // a TYPEDDICTIONARY entry wasn't listed within
    // Variant.hpp (godot-cpp/include/godot_cpp/variant/variant.hpp),
    // I used a regular Dictionary (listed within variant.hpp as
    // DICTIONARY) instead.
    // (Note: Using a set here would still be viable; I'd simply
    // remove all arguments from the "start_game" signal, then access
    // this set via get_node<Hud>("Hud") within main.cpp.)
    {UtilityFunctions::print("Adding player " + strint + 
" to the game.");
        players_to_include[strint] = 0;}
    
    if (input->is_action_pressed("reset_overall_stats_"+strint))
    {
    // double() is used here to prevent an ambiguous-overload
    // error.
    id_reset_time_dict[strint] = double(id_reset_time_dict[strint]) + delta;}
    else // In this case, the time should be reset to 0--as we'll
    // only want to reset the game if a player has held down
    // the button for two consecutive seconds.
    {id_reset_time_dict[strint] = 0.0;}


    // The following code checks to see whether anyone has held down
    // the reset button for at least two consecutive seconds.
    // If so, the game's stats will get reset.

    if (double(id_reset_time_dict[strint]) >= 2.0)

    {UtilityFunctions::print("reset-overall-stats request called.");
    emit_signal("reset_overall_stats");
        id_reset_time_dict[strint] = 0.0; // Prevents redundant
        // reset calls from the same player from getting sent
    }

        // Determining whether a new game should be launched:
    // (Any player can launch a game by holding down both the 
    // Fire button and the Reset_Overall_Stats button on 
    // his/her device.)

    if (
        (input->is_action_pressed("fire_"+strint))
        && (input->is_action_pressed(
        "reset_overall_stats_"+strint))
        && (can_launch_new_game == true) && (announce_winner == false)
    )
    {UtilityFunctions::print("New game requested.");
        can_launch_new_game = false; // Prevents the Fire button from
        // restarting the game once it's active
        show_message(""); // Clears the message field.

        // The following signal will get picked up by main.cpp's
        // on_hud_start_game() function, which will use the 
        // players_to_include value to determine how many players
        // to add to the game.
        winner_message = ""; // Clears out the winner from the previous
        // game (if any)
        emit_signal("start_game", players_to_include);
        break;
    };

}

}

    // Older code that allowed the first player to specify 
    // how many players to include:
    // // This code will only recognize the device with the ID 0 
    // // (to which "fire_0" is linked).
    // if (input->is_action_just_pressed("move_right_0"))
    // {players_to_include = std::min(players_to_include +1, 8);}

    // else if (input->is_action_just_pressed("move_left_0"))
    // {players_to_include = std::max(players_to_include -1, 2);}




    // An alternative setup would be to increment a separate variable
    // (int press_count) by 1 
    // whenever a particular button is pressed, then determine 
    // the number of players to include via the following: (This code
    // will keep players_to_include between 2 and 8, at least as long
    // as press_count doesn't get so large as to effect integer
    // overflow.)
    // players_to_include = 3 + ((press_count -1) % 6);

    // The integer-to-string conversion in the following line was based on 
    // godot-cpp/gen/include/godot_cpp/variant/string.hpp
    // and https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/hud.cpp .

    // Showing a message: (Note that you can use \n to add line
    // breaks to this string. See
    // https://www.reddit.com/r/godot/comments/u37sj1/comment/i4nrizz/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button )


    String instructions = "To join this game, press Fire on \
your controller.\nTo launch a game, press both Fire and\nReset \
simultaneously.\nTo reset overall stats, hold down Reset\nfor \
two seconds.\n\n";

// Displaying which players have been added so far:

String added_players = "";


Array players_to_include_keys = players_to_include.keys();

for (int key_index = 0; 
key_index < players_to_include_keys.size(); key_index++)

{
String added_player = players_to_include_keys[key_index];

added_players += "Player " + added_player + " (" + String(
mnchar_id_color_name_dict[added_player]) + ") has been \
added to the game.\n";

}


    
    // Determining whether to prevent a new game from being started
    // until players have time to review the winner:
    // (This condition will be valid if (1) a previous game has
    // concluded and (2) our message hasn't displayed for at least
    // 2 seconds.
    if ((message_time < 2) && (winner_message != ""))
    {
    announce_winner = true;}

    else {announce_winner = false;}
    
    if (announce_winner == true)
    {show_message(winner_message);}

    else if (can_launch_new_game == true)  
    // In this condition, the results of the previous game
    // will still be shown, but we'll display instructions for
    // launching a new game as well.

    {show_message(winner_message+instructions+added_players);}

    // Based on https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp
    // Note that this code works even though we instantiated Hud
    // as a child scene rather than a node in itself.
    // Also note that, until the announce_winner condition becomes
    // false, the player won't be able to launch a new game.
}

}




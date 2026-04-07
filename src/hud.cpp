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
Variant::INT, "player_count")));
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
    // This code will only recognize the device with the ID 0 
    // (to which "fire_0" is linked).
    if (input->is_action_just_pressed("move_right_0"))
    {players_to_include = std::min(players_to_include +1, 8);}

    else if (input->is_action_just_pressed("move_left_0"))
    {players_to_include = std::max(players_to_include -1, 2);}

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

    String players_to_include_string = String::num(
    players_to_include).substr(0, 1);

    // substr() keeps only the first character within players_to_include_string,
    // thus replacing '3.0' with '3'. This operation is a good fit
    // for this use case because players_to_include will always be a 
    // single-digit number.
    // (Based on godot-cpp/gen/include/godot_cpp/variant/string.hpp)

    // Showing a message: (Note that you can use \n to add line
    // breaks to this string. See
    // https://www.reddit.com/r/godot/comments/u37sj1/comment/i4nrizz/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button )


    String instructions = "Press Fire to launch \
a "+players_to_include_string+"-player game.\nPress left and right \
on Player 0's control stick\nto increase and decrease the \
player count, respectively.";
    
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

    else  // In this condition, the results of the previous game
    // will still be shown, but we'll display instructions for
    // launching a new game as well.

    {show_message(winner_message+instructions);}

    // Based on https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp
    // Note that this code works even though we instantiated Hud
    // as a child scene rather than a node in itself.
    // Also note that, until the announce_winner condition becomes
    // false, the player won't be able to launch a new game.

    if (
        (input->is_action_just_pressed("fire_0")) 
        && (can_launch_new_game == true) && (announce_winner == false)
    )
    {can_launch_new_game = false; // Prevents the Fire button from
        // restarting the game once it's active
        show_message(""); // Clears the message field.
        // The following signal will get picked up by main.cpp's
        // on_hud_start_game() function, which will use the 
        // players_to_include value to determine how many players
        // to add to the game.
        winner_message = ""; // Clears out the winner from the previous
        // game (if any)
        emit_signal("start_game", players_to_include);



    };

}

}


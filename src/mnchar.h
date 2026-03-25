// Based on: https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html
// (But modified for my 3D game)

// I chose to use CharacterBody3D as my player's class because 
// the Your First 3D Game tutorial uses this same class; see
// https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/02.player_input.html 

// Code for the C++ CharacterBody3D class can be found in the 
// following files within a *compiled* godot-cpp repository:

// godot-cpp/gen/src/classes/character_body3d.cpp
// godot-cpp/gen/include/godot_cpp/classes/character_body3d.hpp

#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
// The previous line is based on:
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.h

namespace godot {

class Mnchar : public CharacterBody3D {
	GDCLASS(Mnchar, CharacterBody3D)

private:
	// See: https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html
	double movement_speed; // In meters per second
	// Placed in the private category following the example found at
	// // Based on https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html 
	Ref<PackedScene> projectile_scene; // Based on:
	// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.h
	double rotation_speed = 0.15; 

	//int player_id; // Added to (hopefully) assist with multiplayer
	// code. (My idea is to assign different IDs to different Mnchar
	// instances, either in the editor or (eventually) via code),
	// then have different controllers work for different players
	// depending on their ID. (For example, if the player's ID is
	// 0, the code would then check for move_right_0, move_left_0,
	// etc; if it's 1, it would instead check for move_right_1,
	// move_left_1, and so on.)

protected:
	static void _bind_methods();

public:
	Mnchar();
	~Mnchar();

	// The following two lines are based on:
	// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.h
	Ref<PackedScene> get_projectile_scene();
	void set_projectile_scene(Ref<PackedScene>);


	// The following two lines are based on 
	// https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html .
	void set_movement_speed(const double movement_speed);
	double get_movement_speed() const;
	

	void _physics_process(double delta) override;
    // Using _physics_process rather than
    // _process per the example found at 
    // https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html

	void shoot_projectile();

};

} // namespace godot
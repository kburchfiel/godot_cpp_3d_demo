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

namespace godot {

class Mnchar : public CharacterBody3D {
	GDCLASS(Mnchar, CharacterBody3D)

private:
	double time_passed;

protected:
	static void _bind_methods();

public:
	Mnchar();
	~Mnchar();

	void _physics_process(double delta) override;
    // Using _physics_process rather than
    // _process per the example found at 
    // https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html
};

} // namespace godot
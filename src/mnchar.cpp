// Based on: https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html
// (But modified for my 3D game)

#include "mnchar.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Mnchar::_bind_methods() {
}

Mnchar::Mnchar() {
	// Initialize any variables here.
	time_passed = 0.0;
}

Mnchar::~Mnchar() {
	// Add your cleanup here.
}

void Mnchar::_physics_process(double delta) {
	time_passed += delta;

    // Modified the original code to make it compatible with our 
    // 3D object. 
	Vector3 new_position = Vector3(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)), 0);

	set_position(new_position);
}
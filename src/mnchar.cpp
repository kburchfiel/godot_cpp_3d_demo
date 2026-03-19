// Based on:
// https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html
// (But modified for my 3D game)

#include "mnchar.h"
#include <godot_cpp/core/class_db.hpp>
// The following two lines come from
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/player.cpp
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/variant/basis.hpp>
// Adding in utility functions for printing purposes. Based on:
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/player.cpp
#include <godot_cpp/variant/utility_functions.hpp>
#include "projectile.h"


using namespace godot;

// The following bind_methods code is based on similar code found 
// within https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html .
void Mnchar::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_movement_speed"), &Mnchar::get_movement_speed);
	ClassDB::bind_method(D_METHOD("set_movement_speed", "p_movement_speed"), &Mnchar::set_movement_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "movement_speed"), "set_movement_speed", "get_movement_speed");

  // Adding in code to retrieve our projectile scene so that the player can fire bullets:
  // (This code was based on 
  // https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp ).
    ClassDB::bind_method(D_METHOD("get_projectile_scene"), &Mnchar::get_projectile_scene);
    ClassDB::bind_method(D_METHOD("set_projectile_scene", "projectile_scene"), &Mnchar::set_projectile_scene);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packed_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"), "set_projectile_scene", "get_projectile_scene");
}

// The following two functions were also based on
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp .
Ref<PackedScene> Mnchar::get_projectile_scene() {
    return projectile_scene;
}

void Mnchar::set_projectile_scene(Ref<PackedScene> packed_scene) {  
  projectile_scene = packed_scene;

}

Mnchar::Mnchar() {
  // Initialize any variables here.
  // time_passed = 0.0; // Based on GDExtension 4.6 demo
  movement_speed = 14;
  // Based on
  // https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html
}

Mnchar::~Mnchar() {
  // Add your cleanup here.
}

// The code for set_movement_speed() and get_movement_speed()
// is based on the set_amplitude() and get_amplitude()
// functions within https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html .
void Mnchar::set_movement_speed(const double p_movement_speed) {
  movement_speed = p_movement_speed;
}

void Mnchar::shoot_projectile()

{
  UtilityFunctions::print("shoot_projectile() called within mnchar.cpp.");
  // Retrieving our projectile:
auto projectile = reinterpret_cast<Projectile*>(projectile_scene->instantiate());
// Based on: // https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp

// Initializing the projectile's transform as that 
// of the main character:
Transform3D projectile_transform = Mnchar::get_transform();
// Based on: https://docs.godotengine.org/en/stable/tutorials/3d/using_transforms.html#obtaining-information
// and 

projectile -> start(projectile_transform);
// Based on: https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp

}



double Mnchar::get_movement_speed() const { return movement_speed; }

void Mnchar::_physics_process(double delta) {
  //time_passed += delta;

  Vector3 direction = Vector3(0, 0, 0);
  // Based on
  // https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html
  // and
  // godot_cpp/variant/vector3.hpp

  // This input code is based on
  // https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/player.cpp
  // and
  // https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html
  auto input = Input::get_singleton();
  if (input->is_action_pressed("move_right")) {
    direction.x += 1;
  }
  if (input->is_action_pressed("move_left")) {
    direction.x -= 1;
  }
  if (input->is_action_pressed("move_back")) {
    direction.z += 1;
  }

  if (input->is_action_pressed("move_forward")) {
    direction.z -= 1;
  }

  if (input->is_action_pressed("fire")) {
    shoot_projectile();
  }



  // UtilityFunctions::print("direction's x and z values are now", direction.x, direction.z);

  if (direction != Vector3(0, 0, 0)) {
    direction = direction.normalized();

	// This code was based on some trial and error, and I imagine
	// there's a more elegant approach. (The corresponding GDScript
	// on which it's based was $Pivot.basis = Basis.looking_at(direction),
	// but I wasn't sure whether that Basis value referred to
	// the Node3D's existing basis (which is what my code ended up 
	// using); a new Basis; or Mnchar's basis. 

    get_node<Node3D>("Pivot")->set_basis(get_node<Node3D>(
		"Pivot")->get_basis().looking_at(direction));
  }

  target_velocity.x = direction.x * movement_speed;
  target_velocity.z = direction.z * movement_speed;

  // UtilityFunctions::print("target_velocity's x and z values 
  // are now", target_velocity.x, target_velocity.z);

  // set_velocity() was actually recommended by my compiler!
  // (I had tried just 'velocity', but I'm starting to see that
  // many properties are updated via get_ and set_ methods.)
  set_velocity(target_velocity);

  

  move_and_slide();

  // Modified the original code from the GDExtension tutorial to make it
  // compatible with our 3D object. Vector3 new_position = Vector3(10.0 + (10.0
  // * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)), 0);
  // set_position(new_position);
}
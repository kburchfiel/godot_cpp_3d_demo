// Based in part on:
// https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html
// (But modified for my 3D game)

#include "mnchar.h"
#include <godot_cpp/core/class_db.hpp>
// The following two lines come from
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/player.cpp
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/variant/basis.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <cmath>
// Adding in utility functions for printing purposes. Based on:
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/player.cpp
#include "projectile.h"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

// The following bind_methods code is based on similar code found
// within
// https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html
// .
void Mnchar::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_movement_speed"),
                       &Mnchar::get_movement_speed);
  ClassDB::bind_method(D_METHOD("set_movement_speed", "p_movement_speed"),
                       &Mnchar::set_movement_speed);
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "movement_speed"),
               "set_movement_speed", "get_movement_speed");

  // Adding in code to retrieve our projectile scene so that the player can fire
  // bullets: (This code was based on
  // https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp
  // ).
  ClassDB::bind_method(D_METHOD("get_projectile_scene"),
                       &Mnchar::get_projectile_scene);
  ClassDB::bind_method(D_METHOD("set_projectile_scene", "projectile_scene"),
                       &Mnchar::set_projectile_scene);
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packed_scene",
                            PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
               "set_projectile_scene", "get_projectile_scene");
}

// The following two functions were also based on
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp
// .
Ref<PackedScene> Mnchar::get_projectile_scene() { return projectile_scene; }

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
// functions within
// https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html
// .
void Mnchar::set_movement_speed(const double p_movement_speed) {
  movement_speed = p_movement_speed;
}

void Mnchar::shoot_projectile()

{
  //UtilityFunctions::print("shoot_projectile() called within mnchar.cpp.");
  // Retrieving our projectile:
  auto projectile =
      reinterpret_cast<Projectile *>(projectile_scene->instantiate());
  // Based on: //
  // https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp

  // Initializing the projectile's transform as that
  // of the main character's Pivot node (as it's this node, not
  // Mnchar itself, whose basis we adjust when moving the player):
  Transform3D projectile_transform = get_node<Node3D>(
                                         //"Pivot")->get_global_transform();
                                         "Pivot")
                                         ->get_global_transform();
  // Based on:
  // https://docs.godotengine.org/en/stable/tutorials/3d/using_transforms.html#obtaining-information
  // and https://kidscancode.org/godot_recipes/3.x/3d/3d_shooting/
  // Note that, because we're retrieving the Pivot's transform,
  // we need the global transform rather than the local one--as I think
  // the local one just refers to the pivot's orientation relative
  // to Mnchar.

  // Transform3D projectile_transform = get_transform();

  projectile_transform =
      projectile_transform.translated_local(Vector3(0, 0, 3));

  // The above line also creates some distance in between the projectile and the
  // firer. This prevents the firer from getting hit by its own bullet
  // immediately after firing! For translated_local(), see:
  // https://docs.godotengine.org/en/stable/classes/class_transform3d.html#class-transform3d-method-translated
  // We want translated_local() rather than translate() here because
  // we want to move the projectile in front of the Mnchar's field
  // of view rather than the game area itself.

  projectile->start(projectile_transform);
  // Based on:
  // https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp

  // In order for our bullets to actually appear, we also need to
  // add them to our parent scene (e.g. main.tscn).
  get_parent()->add_child(projectile);
  // See
  // https://docs.godotengine.org/en/stable/classes/class_node.html#class-node-method-get-parent
  // and
  // https://docs.godotengine.org/en/stable/classes/class_node.html#class-node-method-add-child
}

double Mnchar::get_movement_speed() const { return movement_speed; }

void Mnchar::_physics_process(double delta) {
  // The following code allows the player to rotate, strafe,
  // move forward and back, and shoot projectiles.

  // Initializing x (left/right) and z (forward/back) components
  // of the player's velocity:

  float x_direction = 0;
  float z_direction = 0;

  // Based on
  // https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html
  // and
  // godot_cpp/variant/vector3.hpp

  // This input code is based on
  // https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/player.cpp
  // and
  // https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html


  auto input = Input::get_singleton();

  // Specifying x and z directions: (These will range from -1 to 1
  // depending on how strong the player presses a given joystick.)
  // Based on: https://docs.godotengine.org/en/stable/tutorials/2d/2d_movement.html
  // and https://github.com/godotrecipes/characterbody3d_examples/blob/master/mini_tank.gd
  x_direction = input->get_axis("move_left", "move_right");
  z_direction = input->get_axis("move_forward", "move_back");

  // Rotating the player:
  // See: godot-cpp/gen/src/classes/node3d.cpp ,
  // https://docs.godotengine.org/en/stable/tutorials/3d/using_transforms.html ,
  // and https://docs.godotengine.org/en/stable/tutorials/2d/2d_movement.html

get_node<Node3D>("Pivot")->rotate_object_local(Vector3(0, 1, 0),
   rotation_speed * input->get_axis("rotate_right", "rotate_left"));

  // I chose is_action_just_pressed here so that a projectile would
  // only be fired when the button is first registered as pressed.
  // (Otherwise, bullets would keep getting fired while the button
  // was held down.)
  // See:
  // godot-cpp/gen/include/godot_cpp/classes/input.hpp
  if (input->is_action_just_pressed("fire")) {

    shoot_projectile();
  }

  auto player_transform_basis_z =
      get_node<Node3D>("Pivot")->get_transform().get_basis()[2];

  auto player_transform_basis_x =
      get_node<Node3D>("Pivot")->get_transform().get_basis()[0];

  // I'm not sure why, but multiplying the x and z components
  // of the x and z bases, respectively, by -1 was critical for getting
  // the movement code to work. (I first found this out in the process
  // of creating my projectile code.)

  player_transform_basis_x.x *= -1;

  player_transform_basis_z.z *= -1;

  // Initializing the player's velocity:
  // (We'll then adjust its x and z components based on the
  // x_direction and z_direction values calculated earlier.)
	// See /godot-cpp/src/variant/vector3.cpp and
	// godot-cpp/include/godot_cpp/variant/vector3.hpp
	// Based on https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html
  // and and https://github.com/godotrecipes/characterbody3d_examples/blob/master/mini_tank.gd
  Vector3 target_velocity = Vector3(0, 0, 0);


  // I would like Mnchar to be able to move forward,
  // back, left, or right relative to its current position-but only
  // *one* of those directions (e.g. not diagonally). Therefore,
  // I added in code below that finds the absolute value of 
  // the x and z directions, then moves the player along the axis
  // with the greatest absolute value. This isn't strictly necessary,
  // but I find it to make Mnchar's movement somewhat more
  // intuitive.
  // Note that I'm using the standard library's abs() function
  // rather than Godot's, as the latter appeared to truncate
  // values down to the nearest int.
  float abs_z_direction = std::abs(z_direction);
  float abs_x_direction = std::abs(x_direction);

// UtilityFunctions::print("abs(z_direction): ", abs_z_direction, "abs(x_direction): ", abs_x_direction);


// We could also use target_velocity = rather than target_velocity +=;
// however, the existing code will work better in case we ultimately
// want to allow the player to move along the x and z axes
// at the same time.
  if (abs_z_direction >= abs_x_direction)
  {target_velocity +=
      -1 * player_transform_basis_z * z_direction * movement_speed;}
  else {
  target_velocity +=
      -1 * player_transform_basis_x * x_direction * movement_speed;
  }
  //UtilityFunctions::print("player's target velocity:", target_velocity);
	// Based on https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html
  // and and https://github.com/godotrecipes/characterbody3d_examples/blob/master/mini_tank.gd


  set_velocity(target_velocity);

  move_and_slide();
}
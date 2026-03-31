// Based in part on:
// https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html
// (But modified for my 3D game)

#include "mnchar.h"
#include <godot_cpp/core/class_db.hpp>
// The following two lines come from
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/player.cpp
#include <cmath>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/variant/basis.hpp>
// Adding in utility functions for printing purposes. Based on:
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/player.cpp
#include "projectile.h"
#include <godot_cpp/classes/mesh_instance3d.hpp> // Necessary for us
#include <godot_cpp/variant/utility_functions.hpp>
// to access a MeshInstance3D within this script
#include <godot_cpp/classes/base_material3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/standard_material3d.hpp>
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

  ClassDB::bind_method(D_METHOD("get_mnchar_id"), &Mnchar::get_mnchar_id);
  ClassDB::bind_method(D_METHOD("set_mnchar_id", "p_mnchar_id"),
                       &Mnchar::set_mnchar_id);
  ADD_PROPERTY(PropertyInfo(Variant::STRING, "mnchar_id"), "set_mnchar_id",
               "get_mnchar_id");

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

  ClassDB::bind_method(D_METHOD("_on_projectile_detector_body_entered", "node"),
                       &Mnchar::_on_projectile_detector_body_entered);

  // Based on:
  // https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/player.cpp
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
  // movement_speed = 14; // Already set within mnchar.h
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

double Mnchar::get_movement_speed() const { return movement_speed; }

void Mnchar::set_mnchar_id(const String p_mnchar_id) {
  mnchar_id = p_mnchar_id;
}

Color Mnchar::get_character_color() const { return character_color; }

void Mnchar::set_character_color(const Color character_color_arg) {

  // Retrieving the MncharBody's material, then setting its albedo
  // (color) based on the player's ID. (This way, we can specify
  // different colors for different player IDs, which will make
  // different players easier to distinguish.)
  // (I based this code on existing get_node examples and on
  // the .hpp files for the MeshInstance3D and Material classes.
  // godot-cpp/gen/include/godot_cpp/classes/mesh_instance3d.hpp and
  // godot-cpp/gen/include/godot_cpp/classes/material.hpp ;
  // and

  auto mncharbody_mesh_material = get_node<Node3D>("Pivot")
                                      ->get_node<MeshInstance3D>("MncharBody")
                                      ->get_mesh()
                                      ->surface_get_material(0);

  BaseMaterial3D *mncharbody_mesh_material_3d =
      Object::cast_to<BaseMaterial3D>(*mncharbody_mesh_material);

  // Special thanks to RamblingStranger for pointing me to
  // Object::cast_to as a means of converting an object into a
  // new type. (See
  // https://discordapp.com/channels/212250894228652034/342047011778068481/1487545947608322078)

  mncharbody_mesh_material_3d->set_albedo(character_color_arg);

  // Replacing the player's existing material with this new
  // material:

  auto new_mncharbody_mesh_material =
      Object::cast_to<Material>(mncharbody_mesh_material_3d);

  get_node<Node3D>("Pivot")
      ->get_node<MeshInstance3D>("MncharBody")
      ->get_mesh()
      ->surface_set_material(0,
                             new_mncharbody_mesh_material);

// NOTE: It's crucial that you activate the 'local to scene' feature
// of the Mnchar's material. Otherwise, whenever you change the
// material for one player, it will get changed for other players.
// I am very grateful to Tobias Wink at
// https://www.somethinglikegames.de/en/blog/2023/material-synchronization/
// for helping me solve this!
}

String Mnchar::get_mnchar_id() const { return mnchar_id; }


void Mnchar::start(String mnchar_id_arg) {
  // This function will specify, based on its ID argument,
  // the color and starting location for each Mnchar--thus making
  // it easier to differentiate each Mnchar instance
  // from each other.
  // UtilityFunctions::print("start just got called for Mnchar.");
  set_mnchar_id(mnchar_id_arg);
  UtilityFunctions::print("Mnchar id is:", mnchar_id);

  // Initializing a default color that shouldn't actually show
  // up within any characters:
  Color mnchar_color_arg = Color(0.5, 0.5, 0.5, 1);
  Vector3 translate_val = Vector3(0, 0, 0);

  if (mnchar_id_arg == "0")
  {mnchar_color_arg = Color(1.0, 0.0, 0.0, 1);
  translate_val = Vector3(20, 0, -20);}

  else if (mnchar_id_arg == "1")
  {mnchar_color_arg = Color(0.0, 1.0, 0.0, 1);
  translate_val = Vector3(-20, 0, -20);}

  set_character_color(mnchar_color_arg);

  // Moving the Mnchar to its starting point:
  translate(translate_val);

  // Note: I had originally tried this code:
  // get_node<Node3D>("Pivot") -> translate(translate_val);
  // However, this caused issues when multiple characters
  // were added to the scene--possibly because the transforms
  // of the actual Mnchar class weren't getting changed
  // and were thus overlapping with one another. (This caused
  // them to shoot up in the sky, which was both frustrating
  // and hilarious haha)

  // UtilityFunctions::print("Done!");
}

void Mnchar::shoot_projectile()

{
  // UtilityFunctions::print("shoot_projectile() called within mnchar.cpp.");
  //  Retrieving our projectile:
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

void Mnchar::_on_projectile_detector_body_entered(Node3D *node) {
  UtilityFunctions::print(
      "on_body_entered() just got called within mnchar.cpp.");
  queue_free(); // This function is based on
  // https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/mob.cpp
  // and
  // https://docs.godotengine.org/en/stable/classes/class_node.html#class-node-method-queue-free
  // We may not actually need the *node argument here, since
  // we're not making any updates to the node itself.
  // In order for this function to respond to collisions with
  // projectiles, but *not* the ground or the player itself,
  // you'll need to update the layer and mask items within
  // the editor. See README for more details.
}

void Mnchar::_physics_process(double delta) {

  // if (get_mnchar_id() == "0")
  // {set_character_color(Color(1.0, 0.0, 0.0, 1));}

  // else if (get_mnchar_id() == "1")
  // {set_character_color(Color(0.0, 1.0, 0.0, 1));}

  // UtilityFunctions::print("mnchar_id is:", get_mnchar_id());

  // The following code allows the player to rotate, strafe,
  // move forward and back, and shoot projectiles.

  // Initializing x (left/right) and z (forward/back) components
  // of the player's velocity:

  // UtilityFunctions::print("MncharID is", mnchar_id);

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
  // Based on:
  // https://docs.godotengine.org/en/stable/tutorials/2d/2d_movement.html and
  // https://github.com/godotrecipes/characterbody3d_examples/blob/master/mini_tank.gd

  x_direction =
      input->get_axis("move_left_" + mnchar_id, "move_right_" + mnchar_id);
  z_direction =
      input->get_axis("move_forward_" + mnchar_id, "move_back_" + mnchar_id);

  // Rotating the player:
  // See: godot-cpp/gen/src/classes/node3d.cpp ,
  // https://docs.godotengine.org/en/stable/tutorials/3d/using_transforms.html ,
  // and https://docs.godotengine.org/en/stable/tutorials/2d/2d_movement.html

  get_node<Node3D>("Pivot")->rotate_object_local(
      Vector3(0, 1, 0),
      rotation_speed * input->get_axis("rotate_right_" + mnchar_id,
                                       "rotate_left_" + mnchar_id));

  // I chose is_action_just_pressed here so that a projectile would
  // only be fired when the button is first registered as pressed.
  // (Otherwise, bullets would keep getting fired while the button
  // was held down.)
  // See:
  // godot-cpp/gen/include/godot_cpp/classes/input.hpp
  if (input->is_action_just_pressed("fire_" + mnchar_id)) {

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
  // Based on
  // https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html
  // and and
  // https://github.com/godotrecipes/characterbody3d_examples/blob/master/mini_tank.gd
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

  // UtilityFunctions::print("abs(z_direction): ", abs_z_direction,
  // "abs(x_direction): ", abs_x_direction);

  // We could also use target_velocity = rather than target_velocity +=;
  // however, the existing code will work better in case we ultimately
  // want to allow the player to move along the x and z axes
  // at the same time.
  if (abs_z_direction >= abs_x_direction) {
    target_velocity +=
        -1 * player_transform_basis_z * z_direction * movement_speed;
  } else {
    target_velocity +=
        -1 * player_transform_basis_x * x_direction * movement_speed;
  }
  // UtilityFunctions::print("player's target velocity:", target_velocity);
  //  Based on
  //  https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html
  // and and
  // https://github.com/godotrecipes/characterbody3d_examples/blob/master/mini_tank.gd

  set_velocity(target_velocity);

  move_and_slide();
}
// Based on mnchar.cpp. See that file for additional
// references and comments.

#include "projectile.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/basis.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp> 
#include <godot_cpp/classes/base_material3d.hpp>

using namespace godot;

void Projectile::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_projectile_speed"),
                       &Projectile::get_projectile_speed);
  ClassDB::bind_method(D_METHOD("set_projectile_speed", "p_projectile_speed"),
                       &Projectile::set_projectile_speed);
  ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "projectile_speed"),
               "set_projectile_speed", "get_projectile_speed");
}

Projectile::Projectile() { projectile_speed = 64; }

Projectile::~Projectile() {
  // Add your cleanup here.
}

void Projectile::set_projectile_speed(const double p_projectile_speed) {
  projectile_speed = p_projectile_speed;
}

double Projectile::get_projectile_speed() const { return projectile_speed; }

void Projectile::set_firing_mnchar_id(
  const String firing_mnchar_id_arg) {
  firing_mnchar_id = firing_mnchar_id_arg;
}

String Projectile::get_firing_mnchar_id() const { return firing_mnchar_id; }


// The following function is based mostly on:
// https://docs.godotengine.org/en/stable/tutorials/physics/using_character_body_2d.html ,
// https://docs.godotengine.org/en/stable/tutorials/3d/using_transforms.html#obtaining-information,
// and:
// https://kidscancode.org/godot_recipes/3.x/3d/3d_shooting/


// Another resource worth checking out for programming 3D bullets is:
// https://github.com/godotengine/tps-demo/blob/master/player/player.gd

// The argument to the transform parameter below will come from
// the player's global transform. See 
// https://kidscancode.org/godot_recipes/3.x/3d/3d_shooting/
// for more details.

// This C++ projectile code (though it's for a 2D game) could also help:
// https://github.com/vorlac/godot-roguelite/blob/main/src/entity/projectile/projectile.cpp

void Projectile::start(Transform3D transform, String firing_mnchar_id,
Color projectile_color_arg) {

  
  //UtilityFunctions::print("start() just got called for a projectile.");


  // UtilityFunctions::print("transform's basis is", transform.get_basis());

  // set_transform derives from 
  // godot-cpp/gen/src/classes/node3d.cpp (which I may have come
  // across while doing a content search for 'transform').
  set_transform(transform);
  
  // UtilityFunctions::print("transform is now: ", transform);


  auto projectile_basis_z = Projectile::get_transform().get_basis()[2];
  // I found that, when my player was firing projectiles, the
  // projectiles went the right direction within the X (left/right) axis
  // but the opposite direction within the Z (top/bottom) axis. 
  // Therefore, I added in the following line to invert the Z
  // direction of the basis's Z axis. I imagine that some other error 
  // within my code
  // made this setup necessary, so there should be a way to remove
  // the need for this change in the future, 
  // but thankfully it works now!
  projectile_basis_z.z *= -1;

  // There's not a 'z' attribute of a Basis class, but I think
  // the third vector (accessible via [2]) is equivalent to it.
  // see godot-cpp/include/godot_cpp/variant/basis.hpp .
  // This line was also based in part on
  // https://docs.godotengine.org/en/stable/tutorials/3d/using_transforms.html#obtaining-information .

  //UtilityFunctions::print("projectile_basis_z is now:", projectile_basis_z);

  // The following line is based on 
  // https://docs.godotengine.org/en/stable/tutorials/3d/using_transforms.html#obtaining-information .
  projectile_velocity = -1 * projectile_basis_z * projectile_speed;

  //UtilityFunctions::print("projectile_velocity is now: ", projectile_velocity);

    // I had originally attempted to access the 'z' property
    // of the Basis (as shown in
    // https://docs.godotengine.org/en/stable/classes/class_basis.html#class-basis-property-z ),
    // but it looks like, in C++, I'll need to use get_column()
    // (which I found within basis.hpp) to access this element 
    // instead. (The link above specifies that the Z axis is
    // "column 2 of the matrix.")
    // I had also tried to use get_transform()::get_basis(), but that
    // didn't work correctly.


    Projectile::set_firing_mnchar_id(firing_mnchar_id);


  // Changing the projectile's color (using code similar to that
  // within mnchar.cpp):

  auto projectile_mesh_material = get_node<Node3D>("Pivot")
                                      ->get_node<MeshInstance3D>("ProjectileBody")
                                      ->get_mesh()
                                      ->surface_get_material(0);

  BaseMaterial3D *projectile_mesh_material_3d =
      Object::cast_to<BaseMaterial3D>(*projectile_mesh_material);

  // Special thanks to RamblingStranger for pointing me to
  // Object::cast_to as a means of converting an object into a
  // new type. (See
  // https://discordapp.com/channels/212250894228652034/342047011778068481/1487545947608322078)

  projectile_mesh_material_3d->set_albedo(projectile_color_arg);

  // Replacing the player's existing material with this new
  // material:

  auto new_projectile_mesh_material =
      Object::cast_to<Material>(projectile_mesh_material_3d);

  get_node<Node3D>("Pivot")
      ->get_node<MeshInstance3D>("ProjectileBody")
      ->get_mesh()
      ->surface_set_material(0,
                             new_projectile_mesh_material);

}


// The following function is based on 
// https://docs.godotengine.org/en/stable/tutorials/physics/using_character_body_2d.html and
// https://docs.godotengine.org/en/stable/tutorials/3d/using_transforms.html#introduction .
void Projectile::_physics_process(double delta) {

auto collision = move_and_collide(projectile_velocity * delta);
// Based on: https://docs.godotengine.org/en/stable/tutorials/physics/using_character_body_2d.html
// This section could be expanded using the code in the previous
// link. (You'd just need to rework it in C++.)

//UtilityFunctions::print("active_time is", active_time);

// Remove the projectile from the game if it has been active
// for at least two seconds. (This should give it plenty of time
// to travel across the game area, even diagonally from one corner to
// the opposite corner.)
if (active_time >= 2)
{queue_free();}

active_time += delta;



}



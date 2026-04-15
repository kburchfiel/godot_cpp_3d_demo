// Based on mnchar.h. See that file for additional
// references and comments.

#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace godot {

class Projectile : public CharacterBody3D {
  GDCLASS(Projectile, CharacterBody3D)

private:
  double projectile_speed = 64;

  double active_time = 0; // Will store how long a projectile has
  // been active (thus allowing us to delete it after a certain amount
  // of time has passed). This is based on
  // https://docs.godotengine.org/en/stable/tutorials/scripting/idle_and_physics_processing.html

  // The following string will let us determine which Mnchar
  // fired a projectile--and, thus, which Mnchar should be credited for
  // a hit on another Mnchar.
  String firing_mnchar_id = "";

protected:
  static void _bind_methods();

public:
  Projectile();
  ~Projectile();

  Vector3 projectile_velocity = Vector3(0, 0, 0);

  void set_projectile_speed(const double movement_speed);
  double get_projectile_speed() const;

  void set_firing_mnchar_id(const String firing_mnchar_id_arg);
  String get_firing_mnchar_id() const;

  void start(Transform3D transform, String firing_mnchar_id,
             Color projectile_color_arg);
  void _physics_process(double delta) override;
};

} // namespace godot
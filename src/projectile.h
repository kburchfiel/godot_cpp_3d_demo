// Based on mnchar.h. See that file for additional
// references and comments.

#pragma once

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace godot {

class Projectile : public CharacterBody3D {
	GDCLASS(Projectile, CharacterBody3D)

private:
	double time_passed;

	double projectile_speed; 

protected:
	static void _bind_methods();

public:
	Projectile();
	~Projectile();

	Vector3 projectile_velocity = Vector3(0, 0, 0);


	void set_projectile_speed(const double movement_speed);
	double get_projectile_speed() const;
	void start(Transform3D transform);
	void _physics_process(double delta) override;

};

} 
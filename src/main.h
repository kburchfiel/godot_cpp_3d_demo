// Based in part on https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.h
// and on mnchar.h within this project

#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;

class Main: public Node {
    GDCLASS(Main, Node)

    private:

    Ref<PackedScene> mnchar_scene;


protected:
static void _bind_methods();


public:

Main();
~Main();

Ref<PackedScene> get_mnchar_scene();
void set_mnchar_scene(Ref<PackedScene>);

void _ready();


};
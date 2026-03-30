// Based in part on:
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp
// and on my mnchar.cpp code

#include "main.h"
#include "mnchar.h"

void Main::_bind_methods() {


  ClassDB::bind_method(D_METHOD("get_mnchar_0_scene"),
                       &Main::get_mnchar_0_scene);
  ClassDB::bind_method(D_METHOD("set_mnchar_0_scene", "mnchar_0_scene"),
                       &Main::set_mnchar_0_scene);
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packed_scene",
                            PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
               "set_mnchar_0_scene", "get_mnchar_0_scene");

  ClassDB::bind_method(D_METHOD("get_mnchar_1_scene"),
                       &Main::get_mnchar_1_scene);
  ClassDB::bind_method(D_METHOD("set_mnchar_1_scene", "mnchar_1_scene"),
                       &Main::set_mnchar_1_scene);
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packed_scene",
                            PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
               "set_mnchar_1_scene", "get_mnchar_1_scene");

  }


  Ref<PackedScene> Main::get_mnchar_0_scene() { return mnchar_0_scene; }

void Main::set_mnchar_0_scene(Ref<PackedScene> packed_scene) {
  mnchar_0_scene = packed_scene;
}

  Ref<PackedScene> Main::get_mnchar_1_scene() { return mnchar_1_scene; }

void Main::set_mnchar_1_scene(Ref<PackedScene> packed_scene) {
  mnchar_1_scene = packed_scene;
}

Main::Main() {
}

Main::~Main() {
}


void Main::_ready() {
auto mnchar_0 =
    reinterpret_cast<Mnchar*>(mnchar_0_scene->instantiate());
    // Set ID as 0

auto mnchar_1 =
    reinterpret_cast<Mnchar*>(mnchar_1_scene->instantiate());
    // Set ID as 1

}
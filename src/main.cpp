// Based in part on:
// https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/scene/main.cpp
// and on my mnchar.cpp code

#include "main.h"
#include "mnchar.h"

void Main::_bind_methods() {


  ClassDB::bind_method(D_METHOD("get_mnchar_scene"),
                       &Main::get_mnchar_scene);
  ClassDB::bind_method(D_METHOD("set_mnchar_scene", "mnchar_scene"),
                       &Main::set_mnchar_scene);
  ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "packed_scene",
                            PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
               "set_mnchar_scene", "get_mnchar_scene");
  }


  Ref<PackedScene> Main::get_mnchar_scene() { return mnchar_scene; }

void Main::set_mnchar_scene(Ref<PackedScene> packed_scene) {
  mnchar_scene = packed_scene;
}

Main::Main() {
}

Main::~Main() {
}


void Main::_ready() {

auto mnchar_0 =
    reinterpret_cast<Mnchar*>(get_mnchar_scene()->instantiate());
    mnchar_0 -> start("0", Vector3(20, 0, -20));

    add_child(mnchar_0);


auto mnchar_1 =
    reinterpret_cast<Mnchar*>(mnchar_scene->instantiate());
    mnchar_1 -> start("1", Vector3(-20, 0, -20));

    add_child(mnchar_1);


    UtilityFunctions::print("At the end of Main::_ready, \
the mnchar_id values of mnchar_0 and mnchar_1 are \
", mnchar_0 -> get_mnchar_id(), 
" and ", mnchar_1 -> get_mnchar_id());



}
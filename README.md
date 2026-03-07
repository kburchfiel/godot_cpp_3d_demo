## Godot 3D Demo using C++ and GDExtension

(A *very* early work in progress!)

By Ken Burchfiel

Released under the MIT License

This project is an opportunity for me to *learn* how to use GDExtension, together with Godot 4.6, to create a game in C++. For my own reference, I'm planning to note some of the steps I took here; additional comments will be found within the project's src folder (which will contain my game's code).

### References and resources I used to create this project:

* [Version 4.6 of the Getting Started GDExtension documentation page (GDEGS)](https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html)

* [Version 4.6 of the Your first 3D Game tutorial (YF3DG)](https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/index.html) 

    This tutorial doesn't include C++ references, but its GDScript and C# code snippets provide valuable 'hints' about what C++ classes, methods, and functions to use. The instructions provided for setting up a 3D project within Godot were very helpful as well, of course.

* [godot-cpp library (godot-cpp)](https://github.com/godotengine/godot-cpp) 

    The *compiled* version of this library is an indispensable C++ reference.

    Note: I used the latest master version of this repository available on 2026-03-06 (9ae37ac). This particular version can be found [at this link](https://github.com/godotengine/godot-cpp/tree/9ae37ac8b9b14df5284dc3d4bf87e7d8b3327503).


## Part 1: Initial setup

For my initial setup, I started with the source code found in GDEGS, but then updated it to accommodate my 3D setup. For instance, rather than use a Sprite2D as the basis for my 'Mnchar' (main character) class, I used CharacterBody3D, as this was the class referenced in YF3DG. (More on this in the following section.) 

After compiling my code, I updated my project within the Godot editor to include a floor; a camera; a light source; and a shape for the player. (I had created this shape for [a GDNative C++ project back in 2023](https://github.com/kburchfiel/godot_demo_3d_gdnative_cpp_project).) When I added my Mnchar class to the project, I confirmed that it moved in a somewhat-circular fashion as instructed by my code:

    ```
    Vector3 new_position = Vector3(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)), 0);
    ```

    (This code was based on similar code within GDEGS; I simply replaced 'Vector2' with 'Vector3' and added a 0 at the end to represent the third dimension.)

Here's what things looked like at this point:

![](/Screenshots/Initial_Setup.png)




### An aside: Finding C++ code equivalents to GDScript code

When first getting acquainted with C++ in Godot, you might wonder how you can find C++ code equivalents for the GDScript code found within tutorials and other documentation materials. My search for a C++-based CharacterBody3D class shows what this process might look like for you. 

Since YF3DG has GDScript and C# (but not C++) code excerpts, I first needed to double-check the name for this class within the C++ API. A content search within my godot-cpp library for 'characterbody' turned up two relevant code files: 

    * godot_cpp/classes/character_body3d.hpp (I needed to include this file within the C++ code for my main-character file.)

    * godot-cpp/gen/src/classes/character_body3d.cpp

Using these files, I was able to confirm that this class is also titled CharacterBody3D within the C++ API. I also confirmed that this class has the `move_and_slide()` function referenced within YF3DG. (A content search for `move_and_slide` would also have helped me locate the character_body3d.cpp file.)









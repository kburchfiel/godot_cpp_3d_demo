## Godot 3D Demo using C++ and GDExtension

(A *very* early work in progress!)

By Ken Burchfiel

Released under the MIT License

*Note: I chose not to use generative AI tools to write this project's code or this Readme. It was important for me to understand what the code was doing, and I felt that LLMs would get in the way of that.*

This project is an opportunity for me to *learn* how to use GDExtension, together with Godot 4.6, to create a game in C++. For my own reference, I'm planning to note some of the steps I took here; additional comments will be found within the project's src folder (which will contain my game's code).

### References and resources I used to create this project: 

(This list is incomplete; additional resources are mentioned within the code.)

* [Version 4.6 of the Getting Started GDExtension documentation page (GDEGS)](https://docs.godotengine.org/en/4.6/tutorials/scripting/cpp/gdextension_cpp_example.html)

* [Version 4.6 of the Your first 3D Game tutorial (YF3DG)](https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/index.html) 

    This tutorial doesn't include C++ references, but its GDScript and C# code snippets provide valuable 'hints' about what C++ classes, methods, and functions to use. The instructions provided for setting up a 3D project within Godot were very helpful as well, of course.

* [godot-cpp library (godot-cpp)](https://github.com/godotengine/godot-cpp) 

    The *compiled* version of this library is an indispensable C++ reference.

    Note: I used the latest master version of this repository available on 2026-03-06 (9ae37ac). This particular version can be found [at this link](https://github.com/godotengine/godot-cpp/tree/9ae37ac8b9b14df5284dc3d4bf87e7d8b3327503).

* My [C++-based Your First 2D Game tutorial (YF2DGC++)](https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/tree/main)--which is based heavily on [a similar tutorial by J-Dax for Godot 4.3](https://github.com/j-dax/gd-cpp). (That tutorial was released under the BSD 3-Clause license; my copy is released under the MIT license.)


## Part 1: Initial setup

For my initial setup, I started with the source code found in GDEGS, but then updated it to accommodate my 3D setup. For instance, rather than use a Sprite2D as the basis for my 'Mnchar' (main character) class, I used CharacterBody3D, as this was the class referenced in YF3DG. (More on this in the following section.) 

After compiling my code, I updated my project within the Godot editor to include a floor; a camera; a light source; and a shape for the player. (I had created this shape for [a GDNative C++ project back in 2023](https://github.com/kburchfiel/godot_demo_3d_gdnative_cpp_project).) When I added my Mnchar class to the project, I confirmed that it moved in a somewhat-circular fashion as instructed by my code:

    ```
    Vector3 new_position = Vector3(10.0 + (10.0 * sin(time_passed * 2.0)), 10.0 + (10.0 * cos(time_passed * 1.5)), 0);
    ```

    (This code was based on similar code within GDEGS; I simply replaced 'Vector2' with 'Vector3' and added a 0 at the end to represent the third dimension.)

Here's what things looked like at this point:

![](/Screenshots/Initial_Setup.png)


## Part 2: Basic player movement

To get the player to move, I translated some of the GDScript code found in the [Moving the player with code](https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/03.player_movement_code.html) section of YF3DG to C++. (The [player.cpp code within YF2DGC++](https://github.com/kburchfiel/cpp_yf2dg_gd_4pt_6/blob/main/src/entity/player.cpp)--which is [mostly J-Dax's own code](https://github.com/j-dax/gd-cpp/blob/main/src/entity/player.cpp)--proved helpful here.) I skipped the jump-related code and simply worked on getting the player to move left, right, forward, back, and diagonally using the arrow keys. (I was also able to get the player to rotate in the direction of its movement.)

This process also involved adding some movement commands to my project's input map as instructed by YF3DG.

(Note: At one point, the player's `movement_speed` attribute wasn't appearing within my project editor. However, when I later reopened the editor, the attribute then appeared.)

I later modified this code to allow the player to strafe and rotate, thus freeing up many more movement options.


## Part 3: Setting up projectiles

Since Godot's [Third Person Shooter](https://github.com/godotengine/tps-demo) demo uses the CharacterBody3D class for its bullets, I decided to do the same. (Since this is also the class on which my Mnchar (player) class is based, I copied and pasted my mnchar.h and mnchar.cpp code as a basis for my projectile code.)

I added lines of code for my new Projectile class (e.g. `#include "projectile.h"` and `GDREGISTER_CLASS(Projectile);` to register_types.cpp.

Within the editor, I created a new scene (which I named projectile.tscn); chose my Projectile class as the root node; and added a Node3D as a child (which I named Pivot); added a MeshInstance3D to this Node3D. (These steps were based both on https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/02.player_input.html and on https://kidscancode.org/godot_recipes/4.x/3d/rolling_cube/index.html). Since I wanted to make my projectile a green rectangular prism, I turned the MeshInstance3D into a box mesh; added a 'StandardMaterial3D' material; changed its albedo to green (#00FF00); and changed the x, y, and z values of its scale to 0.3, 0.3, and 1, respectively.

Next, I added a CollisionShape3D to the Projectile class; chose a BoxShape3D for its Shape entry; and changed its scale to the same values (0.3, 0.3, and 1 for x, y, and z, respectively) that I had chosen for my MeshInstance3D.

I then added in code that made the projectile.tscn scene accessible to my Mnchar class. This way, the Mnchar class could fire bullets by loading the projectile.tscn file; casting this scene to a projectile; and then initializing the projectile's transform with the player's transform. (A crucial part of this process involved reloading the editor after updating my code; going to the Mnchar entry within my main.tscn scene tree; clicking on the new Packed Scene entry within the Inspector menu; and choosing the projectile.tscn file.)

I then updated my projectile.cpp and Mnchar.cpp code to ensure that the bullets would travel in the same direction that the player was facing. This involved a decent amount of trial and error. Some key steps were (1) to initialize the bullet's transform as the Mnchar's Pivot's local transform (as it's that Pivot, rather than the Mnchar itself, that we adjust when moving the player); (2) to perform a local translate of the pivot's transform so that the bullets would start a little ways away from the player; and (3) to adjust the projectile's basis so that the bullets would travel in the intended direction. I also needed to add the bullet as a child of the player's parent scene so that the player's subsequent movement wouldn't affect the bullet's movement.

![](Screenshots/firing_projectiles.png)

## Part 4: Adding multiplayer support

Now that I had a simple, but functional, player class (Mnchar), I wanted to make it possible for multiple players to control multiple Mnchar objects independently. 

To prepare for multiplayer gameplay, I moved my Mnchar class to its own scene, then added two copies of this scene to main.tscn. At first, the game crashed (with a Signal 11 error code) when I attempted to do so; I eventually realized that this was because the Mnchar code references a Pivot class, and this class needed to be present before I could successfully add my Mnchar class. Thus, I  (1) initialized my Mnchar scene (mnchar.tscn) as a CharacterBody3D node; (2) added a Node3D as a child of this node; (3) renamed that Node3D to 'Pivot'; and (4) changed the CharacterBody3D node to a Mnchar node. 

The easiest option for enabling multiplayer functionality, as shown in some online guides, would be to delegate different parts of the keyboard to different players. However, this wasn't ideal for two main reasons. First, I wanted players to be able to use video game controllers to play the game, and it would be tricky to delegate different parts of the controllers to different actions. (Even if I did so, mischievous players could then just hit the buttons corresponding to the other player's movements anyway!)

But more importantly, I wanted to allow up to 8 players to compete at the same time--which more or less necessitated that I allow the same buttons on different controllers to map to different players' inputs.

I implemented a multiplayer setup as follows. First, I updated my project's input map (accessible via Project --> Project Settings --> Input Map) so that it would have multiple copies of each input. (I started with two copies for a 2-player game, but I can later expand this to eight copies.) I added a numerical suffix to the names of these inputs (e.g. "move_right_0", "move_right_1") in order to distinguish them from one another.

Next, I used those same numbers to specify which device would be linked with that input. (All movements ending in "_0" would be linked to Device 0; all movements ending in "_1" will be linked to Device 1; and so on. There are eight device numbers (ranging from 0 to 7) to choose from.

Here are all of my "_1" inputs: (There's a similar set of "_0" inputs, though those are also mapped to keyboard inputs for convenience's sake.)

![](Screenshots/input_map_section.png)


The final step was to update my code such that each player could be linked to one, and only one, set of these inputs. I first added a new String variable (`mnchar_id`) that could store a unique ID for each Mnchar. I made this variable accessible within the editor using my existing `movement_speed` variable as a reference. I then added this ID as a suffix to my original input commands. Here's an example from `Mnchar::_physics_process()` within mnchar.cpp:

```
  x_direction = input->get_axis("move_left_"+mnchar_id, 
    "move_right_"+mnchar_id);
```

If the character's mnchar_id is 0, the code will use `move_left_0` and `move_right_0` as the basis for its input. Meanwhile, if `mnchar_id` is 1, the code will listen for `move_left_1` and `move_right_1` for its inputs. Since the `_0` movements and `_1` movements are mapped to different devices, this approach allows different devices to get mapped to different players. 

A crucial step here, of course, is to make sure that each Mnchar has a different mnchar_id. I accomplished this within the editor, but I'm quite sure this can be achieved via code as well (which would be helpful if I used C++ code, rather than the editor, to add Mnchar objects to the game area.)

The following resources helped me figure out this approach:

    https://www.reddit.com/r/godot/comments/13ikz4u/best_way_to_handle_controller_input_for_local/

    https://github.com/remram44/godot-multiplayer-example

    https://www.gdquest.com/library/split_screen_coop/

    https://godotassetlibrary.com/asset/QdddqG/multiplayer-input

    https://kidscancode.org/godot_recipes/3.x/2d/splitscreen_demo/index.html


## Part 5: Removing players upon collision with a projectile

This step involved adding a `Mnchar::_on_projectile_detector_body_entered()` function within mnchar.cpp and mnchar.h; creating a new Area3D node within the Mnchar.tscn file in the editor and connecting its `on_body_entered` signal to this function (see screenshots below); and updating the collision layers and masks for the Mnchar and Projectile classes. (The [Jumping and squashing monsters](https://docs.godotengine.org/en/4.6/getting_started/first_3d_game/06.jump_and_squash.html) section of Godot's Your First 3D Game tutorial proved very helpful here.)

![](Screenshots/connecting_body_entered_signal.png)

*Connecting the signal*

![](Screenshots/player_signals.png)

*The player's Signals list following this update*

In addition, I updated my projectile code to remove projectiles from the screen after they have been active for at least one second.

## Part 6: Creating separate colors for each Mnchar and its corresponding projectiles

I wanted to assign different colors to different main-character objects in order to make them easier to distinguish. This first involved recreating the mesh used for my Mnchar object in Godot; I had created it in Blender a while back, but I figured that reconstructing it in Godot would allow for more flexibility. (Since each Mnchar's shape is simply a 2x2x2 cube with a 0.25x0.25x0.25 'turret' attached to the front, this didn't take long to accomplish at all.)

Next, I created a second copy of my mnchar.tscn and projectile.tscn scenes, then assigned _0 and _1 suffixes to their filenames. I made the albedos of the Mnchar and Projectile objects in mnchar_0.tscn and projectile_0.tscn to red; next, I made the Mnchar and Projectile objects in mnchar_1.tscn and projectile_1.tscn green. (I also set the ID of the Mnchar in mnchar_1.tscn to 1; the ID of the Mnchar in mnchar_0.tscn was kept as 0. That way, each player could be controlled independently as discussed earlier.

## Part 7: Simplifying player setup

With help from RamblingStranger on discord (https://discordapp.com/channels/212250894228652034/342047011778068481/1487545947608322078), I was able to update Mnchar colors within my C++ code based on Mnchar IDs. Next, I plan to incorporate a main.cpp script into the game that adds Mnchar characters into the game area and sets their IDs. (This will make my game setup more flexible, as it will ultimately allow players to specify how many main characters to add to the game.)

## Next steps (an incomplete list!)

1. Keep only one copy of projectile and Mnchar scenes, then use main.h and main.cpp scripts to add a certain number of these players to your main-game scene. (Make sure to set each of these Mnchar items' IDs accordingly.)

1. Update your projectile.cpp code such that their colors match those of the Mnchar objects firing them.

1. Create HUD code that (among other things) allows players to specify how many Mnchar entities to add to the scene.

## Troubleshooting notes

* I sometimes found, particularly after compiling my C++ code, that the Projectile.tscn scene would sometimes disappear from my Packed Scene entry within my Mnchar's properties. (An "empty" message would appear in its place.) This would then cause the game to crash if I attempted to fire a projectile, as Godot wouldn't know what scene to use as the basis for projectiles. I'm not sure whether this is due to a glitch within Godot 4.6 or some issue with my own setup, but either way, the fix was thankfully quite simple: I simply had to reload the projectile.tscn scene, a process that takes only a few clicks.

* As mentioned within the Multiplayer section: If you're trying to add a GDExtension class to a scene, make sure that the child nodes referenced by your C++ code (e.g. the Pivot node in the case of the Mnchar class) are present within the editor. See the Multiplayer section for more details.

* My game crashed when I attempted to use a std::Map() to assign colors to players based on their ID. I'll need to look into this further, but in the meantime, I've replaced my Map() code with a simple if/else if setup.

### An aside: Finding C++ code equivalents to GDScript code

When first getting acquainted with C++ in Godot, you might wonder how you can find C++ code equivalents for the GDScript code found within tutorials and other documentation materials. My search for a C++-based CharacterBody3D class shows what this process might look like for you. 

Since YF3DG has GDScript and C# (but not C++) code excerpts, I first needed to double-check the name for this class within the C++ API. A content search within my godot-cpp library for 'characterbody' turned up two relevant code files: 

    * godot_cpp/classes/character_body3d.hpp (I needed to include this file within the C++ code for my main-character file.)

    * godot-cpp/gen/src/classes/character_body3d.cpp

Using these files, I was able to confirm that this class is also titled CharacterBody3D within the C++ API. I also confirmed that this class has the `move_and_slide()` function referenced within YF3DG. (A content search for `move_and_slide` would also have helped me locate the character_body3d.cpp file.)









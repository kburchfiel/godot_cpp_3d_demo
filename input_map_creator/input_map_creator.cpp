// input_map_creator.cpp
// By Ken Burchfiel

// Released under the MIT license

/* This C++ script is designed to help automate the process of 
creating duplicate copies of inputs for different device IDs.
It's designed for a specific project of mine, but can
be repurposed for other programs as well.

For all eight possible device IDs within Godot (0 through 7),
the for loop in the code adds those IDs to a set of entries.
Note that this ID will get added to both the device ID field
*and* a suffix within each action name, thus allowing the various
actions to get differentiated within this code.

To update this code:

1. Create a copy of your project.godot file that you can restore later
in case anything goes wrong with the following steps.

2. Create a set of actions within Godot that use a placeholder ID
(e.g. 1). I suggest that you only include controller inputs rather
than keyboard ones in order to simplify the editing process.

3. Go into your project.godot file (within your project folder--
or whatever you happened to name it, then copy and paste all 
entries for one particular ID that you wish to duplicate into
this code.

4. Esacpe all double quotes within the ID with backslashes 
(e.g. "" --> \"\") so as not to confuse the compiler. (Use find +
replace operations for this step and for step 6 in order to save time.)

5. Add \n\ to the end of each line, save for the last line,
in order to (1) maintain line
breaks and (2) continue the string onto the following line.

6. Replace all instances of your device ID with "+std::to_string(i)+"
so that the for loop can insert each unique ID into the string.

7. Compile the program; ensure that the output looks correct;
and then paste it back into your project.godot file.

8. Make sure the input map looks accurate within Godot--and do a test
run of your different inputs in order to ensure that they're working
correctly.

NOTE: An easier-to-maintain setup would involve (1) reading in an existing
set of inputs; (2) using a replace function to update certain
substrings (e.g. \"device\":1 --> \"device\":"+std::to_string(i)+");
and (3) printing your updated code back to an ofstream. However,
this approach will work for now.

*/

#include <fstream>
#include <iostream>



int main() {

std::ofstream ofs {"../input_map.txt"};

std::string input_string = "";

for (int i = 0; i < 8; i++)
// You probably won't need to add all 8 of these values to your 
// own input map, but just in case, all 8 of them will get
// printed out by the code.
// if this isn't the case, you can update the for loop accordingly. 

{

std::string entry_to_print = "move_left_"+std::to_string(i)+"={\n\
\"deadzone\": 0.2,\n\
\"events\": [Object(InputEventJoypadMotion,\"resource_local_to_scene\":false,\"resource_name\":\"\",\"device\":"+std::to_string(i)+",\"axis\":0,\"axis_value\":-1.0,\"script\":null)\n\
]\n\
}\n\
move_right_"+std::to_string(i)+"={\n\
\"deadzone\": 0.2,\n\
\"events\": [Object(InputEventJoypadMotion,\"resource_local_to_scene\":false,\"resource_name\":\"\",\"device\":"+std::to_string(i)+",\"axis\":0,\"axis_value\":1.0,\"script\":null)\n\
]\n\
}\n\
move_forward_"+std::to_string(i)+"={\n\
\"deadzone\": 0.2,\n\
\"events\": [Object(InputEventJoypadMotion,\"resource_local_to_scene\":false,\"resource_name\":\"\",\"device\":"+std::to_string(i)+",\"axis\":1,\"axis_value\":-1.0,\"script\":null)\n\
]\n\
}\n\
move_back_"+std::to_string(i)+"={\n\
\"deadzone\": 0.2,\n\
\"events\": [Object(InputEventJoypadMotion,\"resource_local_to_scene\":false,\"resource_name\":\"\",\"device\":"+std::to_string(i)+",\"axis\":1,\"axis_value\":1.0,\"script\":null)\n\
]\n\
}\n\
rotate_left_"+std::to_string(i)+"={\n\
\"deadzone\": 0.2,\n\
\"events\": [Object(InputEventJoypadMotion,\"resource_local_to_scene\":false,\"resource_name\":\"\",\"device\":"+std::to_string(i)+",\"axis\":2,\"axis_value\":-1.0,\"script\":null)\n\
]\n\
}\n\
rotate_right_"+std::to_string(i)+"={\n\
\"deadzone\": 0.2,\n\
\"events\": [Object(InputEventJoypadMotion,\"resource_local_to_scene\":false,\"resource_name\":\"\",\"device\":"+std::to_string(i)+",\"axis\":2,\"axis_value\":1.0,\"script\":null)\n\
]\n\
}\n\
fire_"+std::to_string(i)+"={\n\
\"deadzone\": 0.2,\n\
\"events\": [Object(InputEventJoypadMotion,\"resource_local_to_scene\":false,\"resource_name\":\"\",\"device\":"+std::to_string(i)+",\"axis\":5,\"axis_value\":1.0,\"script\":null)\n\
]\n\
}\n\
reset_overall_stats_"+std::to_string(i)+"={\n\
\"deadzone\": 0.2,\n\
\"events\": [Object(InputEventJoypadMotion,\"resource_local_to_scene\":false,\"resource_name\":\"\",\"device\":"+std::to_string(i)+",\"axis\":4,\"axis_value\":1.0,\"script\":null)\n\
]\n\
}";


input_string += entry_to_print+"\n";

}


ofs << input_string;

}


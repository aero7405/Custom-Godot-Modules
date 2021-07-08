# Custom Godot Build
 
This is a repos for all of my custom modules for the Godot Engine.
To install follow the instructions in the docs and paste the module in with the other modules.
https://docs.godotengine.org/en/stable/development/compiling/compiling_for_windows.html
Make sure to compile a custom export template and to specify it in the export settings.
https://docs.godotengine.org/en/stable/development/compiling/compiling_for_windows.html#creating-windows-export-templates
Currently Only Windows is supported for the editor and export presets.

## Midi Input
Allows for input from a midi devices.

#### `start_system_input(int port)`
Takes the port number to use. Usually using `0` is fine here.

Must be the first function called on a new `MidiInput` instance.
Starts collecting midi data and initialises the input system.
Starts the timer for timestamps provided with `get_messages()`.
If it fails to connect to a device it will automatically disable the instance.

#### `get_port_name()`
Returns String with current port name.

#### `is_port_connected(int port)`
Returns `true` if `port` is online and that the current instance is not disabled.

#### `get_messages()`
Returns Array of messages from queue.
Messages contain 4 elements in this order.
 - Note Name: Note and Octave
 - Event Type: Only supports note up and down for now.
 - Velocity: Int -> 0 to 128
 - Timestamp: Total time since `start_system_input()`

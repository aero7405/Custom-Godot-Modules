# Custom Godot Build
 
This is a repos for all of my custom modules for the Godot Engine.
To install follow the instructions in the docs and paste the module in with the other modules.
https://docs.godotengine.org/en/stable/development/compiling/compiling_for_windows.html
Make sure to compile a custom export template and to specify it in the export settings.

Currently Only Windows is supported for the editor and export presets.

## Midi Input
Allows for input from a midi devices.
On every instance of Midi Input the first command called must be `start_system_input(int port)`.
If the connection is lost a new instance must be instanced and if there is no connection when `start_system_input(int port)` is called the instance will disable itself!

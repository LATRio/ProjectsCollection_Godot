# Collection of random projects
Godot version: 4.7.2.stable + godot-cpp(v10.0.0)<br>
Collection of all the projects and experiments, because having to track dozens
of projects is inconvenient.<br>

"GDExtension/" folder:<br>
GDExtension C++ project (or godot-cpp itself) needs to be built at least once to generate bindings.<br>
CMakePresets.json is providing "Debug" and "Release" presets with godot-cpp related variables already set.<br>

Debugging GDExtension C++ on Rider/Clion:<br>
Run/Debug Configurations:<br>
Executable: "/bin/godot", (full path to godot's executable)<br>
Program arguments: "--editor" (optional, if not working on editor plugins)<br>
Working directory: "$SolutionDir$/../" (path to directory that contains "project.godot")

<br>
Assets used:<br>
TBA

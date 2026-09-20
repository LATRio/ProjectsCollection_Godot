# Collection of Random Projects

## Table of Contents
- [Projects](#projects)
  - [Composition](#composition)
- [Godot Version](#godot-version)
- [GDExtension Folder](#gdextension-folder)
- [Debugging GDExtension C++ on Rider/Clion](#debugging-gdextension-c-on-riderclion)
- [Assets Used](#assets-used)

---

## Projects
### Composition
All game systems are implemented as components with a scene's script serving as orchestrator.

Features:
- Input component
  - Very primitive handling of user input. Good enough for now.
- Interactor/Interactable components.
  - `Interactor` can detect and call `interact()` function of `Interactable` components.
- Movement component.
  - Moves based on which direction it must move.
  - Adjusts models direction (TODO: Switch to using animation tree)
  - Also supports setting a target position. Useful for NPC behaviors or scripted cutscenes.
- Third person camera component.
  - Smooth camera movement
  - Adjusts camera height to focus on the character's face when zooms too close.
- Moving platform.
  - Interactable scene. Moves when player interacts with it's "control panel". 
  - Has it's own NavMesh and NPC can step onto it and ride it to the different floor.

TODO list:
- Combat related systems
- NPC/Enemy behavior (probably will be powered by LimboAI behavior addon)
- Inventory system
- Separate Navigation Agent into it's own AI or NPC component.

---

## Godot Version
- **Editor:** 4.7.2.stable
- **GDExtension:** godot-cpp (v10.0.0)

This collection will contain all projects and experiments, as having dozens of separate projects is quite inconvenient.

---

## GDExtension Folder
The `GDExtension/` folder contains the GDExtension C++ project (or godot-cpp itself). It is essential to build this project at least once to generate the necessary bindings.

### CMake Presets
The `CMakePresets.json` file provides both "Debug" and "Release" presets, with godot-cpp related variables already configured.

---

## Debugging GDExtension C++ on Rider/Clion
To set up your debugging environment, follow these configurations (Assuming GDExtension folder was opened):

### Run/Debug Configurations:
- **Executable:** 
  ```plaintext
  /bin/godot  # Full path to Godot's executable
  ```
  
- **Program Arguments:** 
  ```plaintext
  --editor  # Optional, use if not working on editor plugins
  ```

- **Working Directory:** 
  ```plaintext
  $SolutionDir$/../  # Path to the directory containing "project.godot"
  ```

---

## Assets Used
- **TBA** (To Be Added)

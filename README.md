# Collection of Random Projects

## Table of Contents
- [Projects](#projects)
  - [Composition](#composition)
  - [Quest System (WIP)](#quest-system-wip)
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
  - Adjusts model's direction (TODO: Switch to using animation tree)
  - Also supports setting a target position. Useful for NPC behaviors or scripted cutscenes.
- Third person camera component.
  - Smooth camera movement
  - Adjusts camera height to focus on the character's face when zooms too close.
- Moving platform.
  - Interactable scene. Moves when player interacts with it's "control panel". 
  - Has its own NavMesh and NPC can step onto it and ride it to the different floor.

TODO list:
- Combat related systems
- NPC/Enemy behavior (probably will be powered by LimboAI behavior addon)
- Inventory system
- Separate Navigation Agent into its own AI/NPC component.
- More elegant way to connect moving platform's NavMesh to level's NavMesh. Maybe build invisible bridge for crossing over?

### Quest System [WIP]

※ Currently being rewritten into C++ from GDScript.

Implementation of a flexible, extensible advanced quest system.

See [QuestSystem](GDExtension/src/quest_system/README.md) (Too long to fit here, also might be a bit outdated)
Also WIP editor that being worked on inside of `quest_editor_wip` branch.

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

## Debugging GDExtension C++ on Rider
To set up your debugging environment, follow these configurations (Assuming GDExtension folder was opened):

(Clion doesn't support natvis.)

### Run/Debug Configurations:
- **Executable:** # Full path to Godot's executable 
  ```plaintext
  /bin/godot
  ```
  
- **Program Arguments:** # Optional, use if working on editor plugins 
  ```plaintext
  --editor
  ```

- **Working Directory:** # Path to the directory containing "project.godot" 
  ```plaintext
  $SolutionDir$/../
  ```

---

## Assets Used
- [Universal Animation Library - Quaternius (CC0 1.0 Universal)](https://store.godotengine.org/asset/quaternius/universal-animation-library/) 
- **TBA** (To Be Added)

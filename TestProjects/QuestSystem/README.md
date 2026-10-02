# Quest System

---

## Table of Contents
- [JSON layouts](#quest-systems-json-layouts)
- [Entries](#entries)
  - [Questline](#questline)
  - [Quest](#quest)
  - [QuestStep](#queststep)
- [Others](#others)
  - [Actions](#actions)
  - [Conditions](#conditions)
  - [Objectives](#objectives)

---

Implementation of feature-rich advanced quest system.

TODO:
- After main features are implemented and basic testing is done, it will be ported into C++ GDExtension.
- Switch to using Godot's Resources instead of JSON.
	- This allows for easier editing of database using custom editor's plugin.
	- Resource database can be compiled into more compact raw data (the one used right now) for performance and memory efficiency.
	- Should compilation happen on project export or when game launches?
	  - Making it compile during runtime makes modding support easier. And make runtime(in-game) creation/modification of quests possible?
	  - Compiling on export more suited if database isn't meant to be tampered with.
- How to handle repeatable quests? Some condition on when it becomes available again?
- Implement integration with save/load system.

---

# Quest System's JSON layouts

### Shared properties of quest entries
Every Questline, Quest and QuestStep share following properties:
- `id` - (Required) unique identifier.
- `type` - (Required) determines if this entry is Questline, Quest or QuestStep. 
- `available_condition` - (Optional) condition on when entry made available for player. **Default**: `TRUE`
- `activation_condition` - (Optional) condition on when entry becomes completable. **Default**: `TRUE`.
- `completion_condition` - (Optional) condition on when entry becomes completed. **Default**: `ObjectiveCompleted_Condition` on QuestStep, otherwise `AllChildEntriesComplete_Condition`.
- `failure_condition` - (Optional) condition on when entry becomes failed. **Default**: `ObjectiveCompleted_Condition` on QuestStep, otherwise `AllChildEntriesComplete_Condition`.
- `on_available` - (Optional) list of actions to perform when entry becomes available. Ex: spawn quest giver NPC or trigger a cutscene to make player aware of this Questline, Quest or QuestStep. **Default**: no actions.
- `on_active` - (Optional) list of actions to perform when entry becomes completable. **Default**: no actions.
- `on_complete` - (Optional) list of actions to perform when entry get completed. **Default**: `UnlockNextQuestEntry_CommandAction`.
- `on_fail` - (Optional) list of actions to perform when entry get failed. **Default**: `UnlockNextQuestEntry_CommandAction`.

---

## Entries
[Entries folder](./Entries)

Base class for Questlines, Quests and QuestSteps

Entry statuses:
- `UNKNOWN` (Not visible and cannot be completed)
- `AVAILABLE` (Visible but cannot be completed, needs to be accepted/activated first)
- `INPROGRESS` (Visible and can be completed)
- `COMPLETED` (Completion condition was met)
- `CANCELLED` (Quest was accepted but was canceled)
- `FAILED` (Failure condition was met)
- `LOCKED` (Locked out due to some reason)
- `SKIPPED` (Parent entry was closed early without reaching this entry)
- `ERROR` (Used to handle erroneous behaviors)

Notes:
- `UNKNOWN` is default status.
- Can only be set to `CANCELLED` status from outside?
- In order to avoid entry getting marked `SKIPPED`, they must be separated into another questline or become a free quest entry
- Entries exist to make things more readable and organized. But for now they're required.
- Setting status to `AVAILABLE`, `INPROGRESS`, `COMPLETED` or `FAILED` executes their respective actions if set.
- Setting status to `COMPLETED` or `FAILED` triggers refresh on parent entry if present.
- Setting status to `AVAILABLE` or `INPROGRESS` triggers refresh on current entry.
- Refreshing an entry forces:
  - evaluation of `availability_condition` if current status is `UNKNOWN`.
  - evaluation of `activation_condition` if current status is `AVAILABLE`.
  - forces refresh of child entries and evaluates `completion_condition` first and then (if returns false) `failure_condition` if current status is `INPROGRESS`. (Might cause child entry to call refresh on the current entry itself... which is fine, I guess).

TODO:
- Setting status to `COMPLETED`, `CANCELLED`, `FAILED` or `LOCKED` causes all the remaining nested entries to be recursively marked as `SKIPPED`. Maybe add a way to revive some of them later on?
- Do I even need to mark them `SKIPPED`? Currently, this doesn't have any uses. Maybe in the future.

### Questline
[questline_entry.gd](./Entries/questline_entry.gd)

List of Quests.
- Organizes quests into both linear and non-linear progression line(s).
- `completion_condition` - will be set to `AllChildEntriesComplete_Condition` by default.
- `failure_condition` - will be set to `AnyChildEntryFailed_Condition` by default.

Properties:
- `quests` - list of Quest entries.

TODO?: Separate questline(or sub questlines) into their own JSON file and only parse them if conditions are met.

### Quest
[quest_entry.gd](./Entries/quest_entry.gd)

Quest that has a list of steps that needs to be completed before quest itself is marked as completed.

- If part of a questline (aka it's part of free quests), `availability_condition` is optional. Unless always available.
- `completion_condition` - will be set to `AllChildEntriesComplete_Condition` by default.
- `failure_condition` - will be set to `AnyChildEntryFailed_Condition` by default.

Properties:
- `steps` - list of QuestStep entries.

### QuestStep
[queststep_entry.gd](./Entries/queststep_entry.gd)

Individual step of the Quest. Contains only one Objective.

- `completion_condition` - will be set to `ObjectiveCompleted_Condition` by default.
- `failure_condition` - will be set to `ObjectiveFailed_Condition` by default.

---

## Others

ObjectSerializationRegistry was created to assist with deserialization of Actions', Conditions', Objectives' and Entries' JSON data.

### Actions
[Actions folder](./Actions)

TODO: Separate into its own system.

Build-in actions or custom. Perform described action upon execution.

Types of actions:
- QuestSystem specific:
  - `SetEntryStatus_Action` - sets given entry's status.
	- Properties:
	  - `entry_id` - ID of the entry.
	  - `new_status` - new status of the given entry.
- TODO:
  - Cancel a Questline, Quest or QuestStep. Player refused to complete it, but it's not a failure either.
  - Complete a Questline, Quest or QuestStep. Useful if there were many ways to complete quest or questline.
  - Lock a Questline, Quest or QuestStep. Useful if conflicting quest was completed/chosen.

### Conditions
[Conditions folder](./Conditions)

TODO: Separate into its own system.

Condition that returns value of its evaluation. May contain nested condition.

Types of conditions:
- Built-in:
  - `Logical_Condition` - performs logical operation on return values of input conditions. "NOT" requires 1 input, while others 2 or more.
    - Properties:
      - `logical_op` - which logical operation to perform on inputs. (Ex: AND, OR, NOT, NAND, NOR, XOR)
      - `inputs` - list of conditions, results of which need to be inputted into a logical operation.
  - `Comparison_Condition` - compares 2 value where left-hand side value is script defined and right-hand side is number.
    - Properties:
      - `compare_op` - which comparison operation to perform in `lhs` and `rhs` values. Uses expression `<lhs><compare_op><rhs>`.
      - `lhs` - value on the left side of the compare operation.
      - `rhs` - value on the right side of the compare operation.
        
	  Notes: `lhs` and `rhs` may contain a string that resolves into an acquirable game value like player's level.
  - `Objective_Condition` - return `TRUE` is objective is completed.
	- Properties:
	  - `objective` - name of the objective player must complete. Tracks player's activity and maybe some other stuff.
  - `False_Condition` and `True_Condition` - conditions that always return false and true respectively. Useful if specific entry condition needs to be set to true by a different entry's action.

- QuestSystem specific
  - `AllChildEntriesComplete_Condition` - checks if all entries in `quests`/`steps` are of status `COMPLETED`.
  - `AnyChildEntryFailed_Condition` - checks if any entry in `quests`/`steps` are of status `FAILED`.
  - `PreviousSiblingEntryIsCompleted_Condition` - checks if previous entry in the same `quests`/`steps` is completed.
  - `ObjectiveCompleted_Condition` - checks if QuestStep's objective's `is_completed` flag is set.
  - `ObjectiveFailed_Condition` - checks if QuestStep's objective's `is_failed` flag is set.

- TODO:
  - Location condition - must arrive to the specific area or trigger Area3D with specific "name" or metadata.

### Objectives

[Objectives](./Objectives)

An objective of a QuestStep that tracks its own completion. There are ones that attach themselves to the global EventBus's signal and track their own completion that way.

Types of objectives:
- Built-in:
  - `DamagePlayer_Objective` - subscribes itself to the `player_received_damage` signal of the EventBus.
  - `HealPlayer_Objective` - subscribes itself to the `player_received_healing` signal of the EventBus.

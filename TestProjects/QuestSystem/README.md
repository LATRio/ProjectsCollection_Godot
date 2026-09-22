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

---

Implementation of feature-rich advanced quest system.

Statuses (applies to every entry, except for singleton):
- `UNKNOWN` (Not visible and cannot be completed)
- `AVAILABLE` (Visible but cannot be completed)
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
- When entry gets `COMPLETED`, `CANCELLED`, `FAILED` or `LOCKED` for some reason, all the remaining nested entries are recursively marked as `SKIPPED`. Maybe add a way to revive some of them later on?
- In order to avoid entry getting marked `SKIPPED`, they must be separated into another questline or become a free quest entry
- Entries exist to make things more readable and organized. But for now they're required.

TODO:
- Switch to using Godot's Resources instead of JSON.
    - This allows for easier editing of database using custom editor's plugin.
    - Resource database can be compiled into more compact raw data (the one used right now) for performance and memory efficiency.
    - Should compilation happen on project export or when game launches?
      - Making it compile during runtime makes modding support easier. And make runtime(in-game) creation/modification of quests possible?
      - Compiling on export more suited if database isn't meant to be tampered with.
- How to handle repeatable quests? Some condition on when it becomes available again?
- Do I even need to mark them `SKIPPED`? Currently, this doesn't have any uses. Maybe in the future.
- Implement integration with save/load system.

---

# Quest System's JSON layouts

### Shared properties of quest entries
Every Questline, Quest and QuestStep share following properties:
- `id` - (Required) unique identifier.
- `type` - (Required) determines if this entry is Questline, Quest or QuestStep. 
- `file` - (Optional) points to the external location where this entry's data is. Default: None.
- `available_condition` - (Optional) condition on when entry made available for player. Skipped if `activation_condition` isn't present. Required if `file` is present. Default: `TRUE`
- `activation_condition` - (Optional) condition on when entry becomes completable. Default: `TRUE`.
- `completion_condition` - (Optional) condition on when entry becomes completed. Default: `AllCompleted_Condition` on Questline and Quests.
- `failure_condition` - (Optional) condition on when entry becomes failed.
- `on_available` - (Optional) list of actions to perform when entry becomes available. Ex: spawn quest giver NPC or trigger a cutscene to make player aware of this Questline, Quest or QuestStep. Default: no actions.
- `on_active` - (Optional) list of actions to perform when entry becomes completable. Default: no actions.
- `on_complete` - (Optional) list of actions to perform when entry get completed. Default: `UnlockNextQuestEntry_CommandAction`.
- `on_fail` - (Optional) list of actions to perform when entry get failed. Default: `UnlockNextQuestEntry_CommandAction`.

---

## Entries
[Entries folder](./Entries)

### Questline
[questline_entry.gd](./Entries/questline_entry.gd)

List of Quests.
- Organizes quests into both linear and non-linear progression line(s).
- Contains Dependency(Relationship) conditions for its quests: decides which route to take based on configurable conditions.
- Has reveal and activation condition.

Properties:
- `quests` - list of Quest entries.

TODO?: Separate questline(or sub questlines) into their own JSON file and only parse them if conditions are met.

### Quest
[quest_entry.gd](./Entries/quest_entry.gd)

List of QuestSteps.
- Manages when which QuestStep should be made available to the player. 
- May be part of a questline or a free quest. Free quests are available globally and must have a prerequisite condition unless they're always available.
- If part of a questline, doesn't need a prerequisite condition. But may have additional conditions (Ex: Level requirement). Allows player's to be aware of this quest's existence and see it's requirements.

Properties:
- `steps` - list of QuestStep entries.

### QuestStep
[queststep_entry.gd](./Entries/queststep_entry.gd)

Individual step of the Quest.
- Only needs `completion_condition`. May or may not have a reward.

---

## Others

ObjectSerializationRegistry was created to assist with deserialization of Actions', Conditions' and Entries' JSON data.

### Actions
[Actions folder](./Actions)

TODO: Separate into its own system.

Build-in actions or custom. Perform described action upon execution.

Types of actions:
- QuestSystem specific:
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

- QuestSystem specific
  - Questline specific condition:
    - `AllQuests{Status}_Condition` - checks if all Quests in `quests` are of status {Status}.
  - Quest specific condition:
    - `AllQuestSteps{Status}_Condition` - checks if all QuestSteps in `steps` are of status {Status}.
    - `LocalQuest{Status}_Condition` - check if sibling Quest is of status {Status}.
  - QuestStep specific condition:
    - `LocalQuestStep{Status}_Condition` - check if sibling QuestStep is of status {Status}.
  - Misc conditions:
    - `Questline{Status}_Condition` - checks if specific questline is of status {Status}.
    - `ExternalQuestStep{Status}_Condition` - checks if specific external(other quest's) quest step is of status {Status}.

- Other:
  - Location condition - must arrive to the specific area or trigger Area3D with specific "name" or metadata.

# Turn-Based Combat

---

## Table of Contents

---

Basic Turn-based combat.

---

# Building blocks

## Combatants

Terms:
- Act - combatant's individual action. Starts when player gets control and gets to choose an action (or when Stun debuff activates and skips).
- Turn - ends when all combatants had acted. In ATB, it has timer a timer that progresses a turn (ake triggers all turn based effects like "process debuffs when turn ends").

Combatant properties:
- Faction (playable, neutral, enemy, some third party)
- Stats:
  - HP
  - SP
  - TP?
  - ATK
  - MATK
  - DEF
  - MDEF
  - AGI (speed of readiness)
  - Equipment slots (As many as dev wants).

    Each slot filters by an equipment category. Each slot accepts only one equipment. 

    Examples:
    - Head
    - Torso
    - Hands
    - Boots
    - Accessories (As many as dev wants)
- List of actions
  - Category (dev-defined)(basic attack, magic attack, etc. or an array of actions)


- Equipment: (CAN BE IMPLEMENTED LATER)
    - Category (dev-defined)
    - Modifying Stats (changes that will apply when equipped to the matching stat)
        - Should think about behavior when required stat isn't present in a combatant equipping it...
    - Stats (stats of the equipment item itself, maybe make it a component?)


- Lingering effects: (CAN BE IMPLEMENTED LATER)
    - Behavior: (allow it to have multiple behaviors?)
        - When act begins
        - When act ends
        - Condition (certain condition needs to be fulfilled during combat for it's effect to manifest?)
        - Passive (active at all times, usually used to affect stats)



## Timer system

Increments readiness of each combatant based on their speed. Or should this be handled using DeltaTime?

## Combat Manager

Manages when combatant becomes ready, they are added into queue of combatants (useful if "Active Time Battle" combat get implemented)

Maybe also have Paper Mario, Expedition 33 style dodge/parry mechanic? But this can be implemented separately after everything is done and working, I think.

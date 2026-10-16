# Item System

---

Can add and remove items from inventory. All items are stored in stacks within inventory.

Each stack track quantity of the item and ItemData, that contains basic info about a specific item.

Control keys:
- "-" - remove item
- "=" - add item

---

`ItemData : Resource` - contains base info about the item:
- `StringName id` - identifier
- `String itemName` - display name of the item
- `String itemDesc` - display description of the item
- `Texture icon` - ref to texture. Optional if icon's name is same as id
- `int maxStackSize` - limit before new ItemStack is created
Optional (or for derived):
+ `int price` - used for trading. Maybe it needs separate buy and sell prices.
+ `float weight` - how much item weights

`ItemStack : RefCounted` - runtime container of ItemData and quantity
- `ItemData itemData` or `StringName itemData` - reference to the actual item
- `int quantity` - amount of items in this stack. Must be <= than maxStackSize.

`Inventory : RefCounted` - actual list container of `ItemStack`s
- `Vector<ItemStack> items` - actual list of `ItemStack`s.
  - `InventorySlot` - if each slot has requirements.

Maybe:
- `ItemDatabase : Object singleton` - contains list of `ItemData`s and helper functions to retrieve misc info.

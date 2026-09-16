class_name InventoryComponent
extends Node
# May be attached to anything: player, enemy, chest, crate...
# Item ID points to the item withing global ItemDatabase that stores each item's
# info/metadata present in the game.
# TODO: Implement ItemDatabase
# Requires: Interaction components.

var items: Dictionary[int, int] # {1: 10, 2: 10} - key as item ID, value as item count


func _ready() -> void:
	# check if interaction related component(s) are present
	pass


func add_item(item_id: int, item_count: int) -> bool: # fails if exceeds max item_count allowed
	#assert(ItemDatabase.is_valid_id(item_id), "[Inventory] Trying to add an item with invalid ID.")
	assert(item_count >= 0, "[Inventory] Trying to add negative amount of items. Use remove_item() instead.")
	#if(ItemDatabase.get_item_max_count() > items[item_id] + item_count)
		#return false
	items[item_id] += item_count
	return true


func remove_item(item_id: int, item_count: int) -> void:
	#assert(ItemDatabase.is_valid_id(item_id), "[Inventory] Trying to remove an item with invalid ID.")
	assert(items.has(item_id), "[Inventory] Tried to remove an item that doesn't exist in this inventory.")
	assert(items[item_id] >= item_count, "[Inventory] Tried to remove more than present in this inventory.")
	items[item_id] -= item_count
	if items[item_id] == 0:
		items.erase(item_id)

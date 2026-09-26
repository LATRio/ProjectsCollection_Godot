extends Node

# TODO: Explore other ways to approach event handling.

# Player events
@warning_ignore("unused_signal")
signal player_received_damage(amount: float)
@warning_ignore("unused_signal")
signal player_received_healing(amount: float)
@warning_ignore("unused_signal")
signal player_died

# Inventory events
@warning_ignore("unused_signal")
signal recieved_item(item_id: int, amount: int)

# NPC events

# TimeSystem events

# World trigger events (if player or some npc walks into a specific area)

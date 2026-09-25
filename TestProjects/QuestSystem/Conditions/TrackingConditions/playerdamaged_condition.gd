class_name PlayerDamaged_Condition
extends Condition

# Maybe make this a part of Comparison_Condition?
# compare_op will be ">"
# lhs will be PlayerDamagedAmount
# rhs will be 0
# This will reduce mount of condition scripts that have to be implemented
# TODO: Also add GameState singleton to track some trackable
# values stored in a Dictionary

static var type := "PlayerDamaged"

var player_got_damaged := false


func evaluate() -> bool:
	return player_got_damaged


func _on_player_receiver_damage(_amount: float) -> void:
	player_got_damaged = true
	notify_parent()


func activate_tracking() -> void:
	EventBus.player_received_damage.connect(_on_player_receiver_damage)


func deactivate_tracking() -> void:
	EventBus.player_received_damage.disconnect(_on_player_receiver_damage)


static func deserialize(_json: Dictionary) -> PlayerDamaged_Condition:
	var condition := PlayerDamaged_Condition.new()
	condition.is_tracking = true
	condition.activate_tracking()
	return condition

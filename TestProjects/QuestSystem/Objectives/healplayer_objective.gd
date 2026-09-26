class_name HealPlayer_Objective
extends Objective

static var type := "HealPlayer"


func register_tracker() -> void:
	EventBus.player_received_healing.connect(_on_objective_updated)


func unregister_tracker() -> void:
	EventBus.player_received_healing.disconnect(_on_objective_updated)


func _on_objective_updated(args: Variant) -> void:
	if typeof(args) == TYPE_FLOAT and args > 0.0:
		_on_objective_completed()


static func deserialize(_json: Dictionary) -> HealPlayer_Objective:
	return HealPlayer_Objective.new()

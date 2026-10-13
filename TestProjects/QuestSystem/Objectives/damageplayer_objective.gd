class_name DamagePlayer_Objective
extends Objective

static func _get_type() -> String:
	return "DamagePlayer"


func _register_tracker() -> void:
	EventBus.player_received_damage.connect(_on_objective_updated)


func _unregister_tracker() -> void:
	EventBus.player_received_damage.disconnect(_on_objective_updated)


func _on_objective_updated(args: Variant) -> void:
	if typeof(args) == TYPE_FLOAT and args > 0.0:
		super._on_objective_completed()


static func _deserialize(_json: Dictionary) -> DamagePlayer_Objective:
	var damage_player := DamagePlayer_Objective.new()
	return damage_player

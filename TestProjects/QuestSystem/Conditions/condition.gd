class_name Condition
extends RefCounted

var caller_id: int
var is_tracking := false


func evaluate() -> bool:
	return true


# Forces parent entry to reevaluate their conditions.
# Only called by 'tracking' conditions
func notify_parent() -> void:
	QuestSystem.get_entry(is_tracking).refresh_entry()


# Only called by 'tracking' conditions
func activate_tracking() -> void:
	pass


# Only called by 'tracking' conditions
func deactivate_tracking() -> void:
	pass

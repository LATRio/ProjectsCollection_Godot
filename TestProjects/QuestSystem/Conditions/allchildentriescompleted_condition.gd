class_name AllChildEntriesComplete_Condition
extends Condition

static func _get_type() -> String:
	return "AllChildEntriesComplete"


func _evaluate() -> bool:
	var caller := QuestSystem.get_entry(caller_id)
	for child_id in caller.get_child_entry_ids():
		if not QuestSystem.get_entry(child_id).get_status() == QuestSystem.ENTRY_STATUS_COMPLETED:
			return false
	return true


static func _deserialize(_json: Dictionary) -> AllChildEntriesComplete_Condition:
	return AllChildEntriesComplete_Condition.new()

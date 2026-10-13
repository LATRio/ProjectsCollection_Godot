class_name PreviousSiblingEntryIsCompleted_Condition
extends Condition

static func _get_type() -> String:
	return "PreviousSiblingEntryIsCompleted"


func _evaluate() -> bool:
	var caller := QuestSystem.get_entry(caller_id)
	var sibling_id := caller.get_previous_sibling_entry_id()
	if not sibling_id:
		return false
	var prev_entry := QuestSystem.get_entry(sibling_id)
	return prev_entry.get_status() == QuestSystem.ENTRY_STATUS_COMPLETED


static func _deserialize(_json: Dictionary) -> PreviousSiblingEntryIsCompleted_Condition:
	return PreviousSiblingEntryIsCompleted_Condition.new()

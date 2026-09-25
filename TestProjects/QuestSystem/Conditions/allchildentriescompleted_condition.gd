class_name AllChildEntriesComplete_Condition
extends Condition

static var type := "AllChildEntriesComplete"


func evaluate() -> bool:
	var caller := QuestSystem.get_entry(caller_id)
	for child_id in caller.get_child_entry_ids():
		if not QuestSystem.get_entry(child_id).get_status() == QuestSystem.EntryStatus.COMPLETED:
			return false
	return true


static func deserialize(_json: Dictionary) -> AllChildEntriesComplete_Condition:
	return AllChildEntriesComplete_Condition.new()

class_name AnyChildEntryFailed_Condition
extends Condition

static var type := "AnyChildEntryFailed"


func evaluate() -> bool:
	var caller := QuestSystem.get_entry(caller_id)
	for child_id in caller.get_child_entry_ids():
		if QuestSystem.get_entry(child_id).get_status() == QuestSystem.EntryStatus.FAILED:
			return true
	return false


static func deserialize(_json: Dictionary) -> AnyChildEntryFailed_Condition:
	return AnyChildEntryFailed_Condition.new()

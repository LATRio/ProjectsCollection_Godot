class_name PreviousSiblingEntryIsCompleted_Condition
extends Condition

static var type := "PreviousSiblingEntryIsCompleted"


func evaluate() -> bool:
	var parent := QuestSystem.get_entry(caller_id)
	var prev_entry := QuestSystem.get_entry(parent.get_previous_sibling_entry())
	return prev_entry.get_status() == QuestSystem.EntryStatus.COMPLETED


static func deserialize(_json: Dictionary) -> PreviousSiblingEntryIsCompleted_Condition:
	return PreviousSiblingEntryIsCompleted_Condition.new()

class_name QuestStepEntry
extends Entry

var objective: Objective

var _parent_quest_id: int # Avoid cyclical referencing of Quest<->QuestStep


func set_status(new_status: QuestSystem.EntryStatus) -> void:
	print("QuestStep[{0}] of Quest[{1}] was set to status {2}", id, _parent_quest_id, new_status)
	super.set_status(new_status)

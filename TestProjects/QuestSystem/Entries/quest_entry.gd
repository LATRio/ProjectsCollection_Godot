class_name QuestEntry
extends Entry

var steps: Dictionary[int, QuestStepEntry]

var _parent_questline_id: int # Avoid cyclical referencing of Questline<->Quest


func set_status(new_status: QuestSystem.EntryStatus) -> void:
	print("Quest[{0}] of Questline[{1}] was set to status {2}", id, _parent_questline_id, new_status)
	super.set_status(new_status)
	if new_status >= QuestSystem.EntryStatus.COMPLETED:
		for step in steps.values():
			if step.status < QuestSystem.EntryStatus.COMPLETED:
				step.set_status(QuestSystem.EntryStatus.SKIPPED)

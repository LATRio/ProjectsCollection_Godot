class_name QuestlineEntry
extends Entry

var quests: Array[QuestEntry]


func set_status(new_status: QuestSystem.EntryStatus) -> void:
	print("Questline[{0}] was set to status {1}", id, new_status)
	super.set_status(new_status)
	if new_status >= QuestSystem.EntryStatus.COMPLETED:
		for quest in quests:
			if quest.status < QuestSystem.EntryStatus.COMPLETED:
				quest.set_status(QuestSystem.EntryStatus.SKIPPED)

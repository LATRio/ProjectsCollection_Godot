class_name QuestlineEntry
extends Entry

static var type := "questline"

var quests: Array[int]


func set_status(new_status: QuestSystem.EntryStatus) -> bool:
	if super.set_status(new_status):
		# Also set statuses of all it's steps
		if new_status >= QuestSystem.EntryStatus.COMPLETED:
			for quest_id in quests:
				if QuestSystem.get_entry_status(quest_id) < QuestSystem.EntryStatus.COMPLETED:
					QuestSystem.set_entry_status(quest_id, QuestSystem.EntryStatus.SKIPPED)
		return true
	return false


static func deserialize(json: Dictionary) -> QuestlineEntry:
	if not json.has("steps"):
		push_error("QuestStep json entry doesn't have 'objective' key.")
		return null
	var questline := QuestlineEntry.new()
	super.deserialize_json(questline, json)
	for quest in json["quests"]:
		var quest_obj: QuestEntry = ObjectSerializationRegistry.deserialize_json(quest)
		quest_obj.parent_id = questline.id
		questline.quests.push_back(quest_obj.id)
		QuestSystem.add_quest(quest_obj)
	return questline

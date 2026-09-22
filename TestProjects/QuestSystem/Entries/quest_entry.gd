class_name QuestEntry
extends Entry

static var type := "quest"

var steps: Array[int] # Stores steps in order they're defined in JSON file


func add_step(step: QuestStepEntry) -> void:
	steps.push_back(step.id)
	QuestSystem.add_step(step)


func set_status(new_status: QuestSystem.EntryStatus) -> bool:
	if super.set_status(new_status):
		# Also set statuses of all it's steps
		if new_status >= QuestSystem.EntryStatus.COMPLETED:
			for step_id in steps:
				if QuestSystem.get_entry_status(step_id) < QuestSystem.EntryStatus.COMPLETED:
					QuestSystem.set_entry_status(step_id, QuestSystem.EntryStatus.SKIPPED)
		return true
	return false


func get_next_sibling_entry() -> int:
	var questline := QuestSystem.get_questline(parent_id)
	if not questline:
		push_error("[QuestSystem] Failed to retrieve a parent Quest[{0}] of QuestStep[{1}]!", parent_id, id)
	for idx in [0, questline.quests.size() - 1]: # if this step is last, return null
		if questline.quests[idx] == id:
			return questline.quests[idx + 1]
	push_error("[QuestSystem] QuestStep[{0}] of Quest[{1}] doesn't have a next sibling!", id, parent_id)
	return -1


static func deserialize(json: Dictionary) -> QuestEntry:
	if not json.has("steps"):
		push_error("QuestStep json entry doesn't have 'objective' key.")
		return null
	var quest := QuestEntry.new()
	super.deserialize_json(quest, json)
	if quest.on_complete.is_empty():
		quest.on_complete.push_back(ActivateNextSiblingEntry_Action.new())
	for step in json["steps"]:
		var step_obj: QuestStepEntry = ObjectSerializationRegistry.deserialize_json(step)
		step_obj.parent_id = quest.id
		quest.steps.push_back(step_obj.id)
		QuestSystem.add_queststep(step_obj)
	return quest

class_name QuestStepEntry
extends Entry

static var type := "queststep"

var objective: Objective


func set_status(new_status: QuestSystem.EntryStatus) -> bool:
	if super.set_status(new_status):
		# Also set statuses of all it's steps
		if new_status == QuestSystem.EntryStatus.INPROGRESS:
			objective.register_tracker()
		if new_status >= QuestSystem.EntryStatus.COMPLETED:
			objective.unregister_tracker()
		return true
	return false


func get_previous_sibling_entry() -> int:
	var quest := QuestSystem.get_quest(parent_id)
	if not quest:
		push_error("[QuestSystem] Failed to retrieve a parent Quest[{0}] of QuestStep[{1}]!".format([parent_id, id]))
	if quest.steps.size() < 2:
		return -1
	for idx in range(1, quest.steps.size()):
		if quest.steps[idx] == id:
			return quest.steps[idx - 1]
	return -1


func get_next_sibling_entry() -> int:
	var quest := QuestSystem.get_quest(parent_id)
	if not quest:
		push_error("[QuestSystem] Failed to retrieve a parent Quest[{0}] of QuestStep[{1}]!".format([parent_id, id]))
	if quest.steps.size() < 2:
		return -1
	for idx in range(0, quest.steps.size() - 1):
		if quest.steps[idx] == id:
			return quest.steps[idx + 1]
	return -1


static func deserialize(json: Dictionary) -> QuestStepEntry:
	if not json.has("objective"):
		push_error("[JSONDeserializer] QuestStep JSON entry doesn't have 'objective' key. JSON: ", json)
		return null
	var queststep := QuestStepEntry.new()
	super.deserialize_json(queststep, json)
	queststep.objective = ObjectSerializationRegistry.deserialize_json(json["objective"])
	queststep.objective.parent_step_id = queststep.id
	return queststep

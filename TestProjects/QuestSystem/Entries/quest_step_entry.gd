class_name QuestStepEntry
extends Entry

static var type := "queststep"

var objective: Objective


func get_next_sibling_entry() -> int:
	var quest := QuestSystem.get_quest(parent_id)
	if not quest:
		push_error("[QuestSystem] Failed to retrieve a parent Quest[{0}] of QuestStep[{1}]!", parent_id, id)
	for idx in [0, quest.steps.size() - 1]: # if this step is last, return null
		if quest.steps[idx] == id:
			return quest.steps[idx + 1]
	push_error("[QuestSystem] QuestStep[{0}] of Quest[{1}] doesn't have a next sibling!", id, parent_id)
	return -1


static func deserialize(json: Dictionary) -> QuestStepEntry:
	if not json.has("objective"):
		push_error("[QuestDatabase] QuestStep JSON entry doesn't have 'objective' key. JSON: ", json)
		return null
	var queststep := QuestStepEntry.new()
	super.deserialize_json(queststep, json)
	if queststep.on_complete.is_empty():
		queststep.on_complete.push_back(ActivateNextSiblingEntry_Action.new())
	return queststep

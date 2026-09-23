class_name SetEntryStatus_Action
extends Action

static var type := "SetEntryStatus"

var entry_id: int
var new_status: QuestSystem.EntryStatus


func execute(_caller_id: int) -> void:
	QuestSystem.set_entry_status(entry_id, new_status)


static func deserialize(json: Dictionary) -> SetEntryStatus_Action:
	if not json.has("entry_id"):
		push_error("[JSONDeserializer] SetEntryStatus_Action JSON entry doesn't have 'entry_id' key. JSON: ", json)
		return null
	if not json.has("new_status"):
		push_error("[JSONDeserializer] SetEntryStatus_Action JSON entry doesn't have 'new_status' key. JSON: ", json)
		return null
	var set_questline_status := SetEntryStatus_Action.new()
	set_questline_status.entry_id = json["entry_id"]
	set_questline_status.new_status = json["new_status"]
	return set_questline_status

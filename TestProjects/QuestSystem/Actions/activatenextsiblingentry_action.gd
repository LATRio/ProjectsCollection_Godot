class_name ActivateNextSiblingEntry_Action
extends Action

static var type := "ActivateNextSiblingEntry"


func execute() -> void:
	var entry := QuestSystem.get_entry(caller_id)
	if entry:
		var sibling_id := entry.get_next_sibling_entry()
		if not sibling_id:
			return
		QuestSystem.set_entry_status(sibling_id, QuestSystem.EntryStatus.INPROGRESS)
	else:
		push_error("[QuestSystem] Failed to execute [ActivateNextSiblingEntry_Action] called by Entry ID [{0}] caller that doesn't exist in database!".format([caller_id]))


static func deserialize(_json: Dictionary) -> ActivateNextSiblingEntry_Action:
	return ActivateNextSiblingEntry_Action.new()

class_name SetQuestlineStatus_Action
extends Action

var questline_id: int
var new_status: QuestSystem.EntryStatus


func execute() -> void:
	QuestSystem.set_questline_status(questline_id, new_status)

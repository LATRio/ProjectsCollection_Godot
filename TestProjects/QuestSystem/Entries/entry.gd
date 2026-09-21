class_name Entry
extends RefCounted

var id: int
var type: String
var status: QuestSystem.EntryStatus
var availability_condition: Condition
var activation_condition: Condition
var completion_condition: Condition
var on_available: Array[Action]
var on_active: Array[Action]
var on_complete: Array[Action]
var on_fail: Array[Action]


func set_status(new_status: QuestSystem.EntryStatus) -> void:
	status = new_status

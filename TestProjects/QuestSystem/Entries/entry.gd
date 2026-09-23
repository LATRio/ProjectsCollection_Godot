class_name Entry
extends RefCounted

var id: int
var status := QuestSystem.EntryStatus.UNKNOWN
var availability_condition: Condition
var activation_condition: Condition
var completion_condition: Condition
var failure_condition: Condition
var on_available: Array[Action]
var on_active: Array[Action]
var on_complete: Array[Action]
var on_fail: Array[Action]

var parent_id: int = -1

func set_status(new_status: QuestSystem.EntryStatus) -> bool:
	if status == new_status:
		return false
	QuestSystem.sort_entry_by_status(id, status, new_status)
	status = new_status
	match new_status:
		QuestSystem.EntryStatus.AVAILABLE:
			if not on_available.is_empty():
				for action in on_available:
					action.execute(id)
		QuestSystem.EntryStatus.INPROGRESS:
			if not on_active.is_empty():
				for action in on_active:
					action.execute(id)
		QuestSystem.EntryStatus.COMPLETED:
			if not on_complete.is_empty():
				for action in on_complete:
					action.execute(id)
		QuestSystem.EntryStatus.FAILED:
			if not on_fail.is_empty():
				for action in on_fail:
					action.execute(id)
		_:
			pass
	return true


func get_status() -> QuestSystem.EntryStatus:
	return status


func get_parent_id() -> int:
	return parent_id


func get_next_sibling_entry() -> int:
	return -1


func test_for_completion() -> void:
	if completion_condition.evaluate():
		set_status(QuestSystem.EntryStatus.COMPLETED)


func test_for_availability() -> void:
	if availability_condition.evaluate():
		set_status(QuestSystem.EntryStatus.AVAILABLE)


func test_for_activation() -> void:
	if activation_condition.evaluate():
		set_status(QuestSystem.EntryStatus.INPROGRESS)


func test_for_failure() -> void:
	if failure_condition.evaluate():
		set_status(QuestSystem.EntryStatus.FAILED)


static func deserialize_json(entry: Entry, json: Dictionary) -> void:
	if not json.has("id"):
		push_error("[JSONDeserializer] Couldn't find key 'id' in entry JSON: ", json)
		return
	entry.id = json["id"]
	if json.has("availability_condition"):
		entry.availability_condition = ObjectSerializationRegistry.deserialize_json(json["availability_condition"])
	if json.has("activation_condition"):
		entry.activation_condition = ObjectSerializationRegistry.deserialize_json(json["activation_condition"])
	if json.has("completion_condition"):
		entry.completion_condition = ObjectSerializationRegistry.deserialize_json(json["completion_condition"])
	if json.has("failure_condition"):
		entry.failure_condition = ObjectSerializationRegistry.deserialize_json(json["failure_condition"])
	if json.has("on_available"):
		for action in json["on_available"]:
			entry.on_available.push_back(ObjectSerializationRegistry.deserialize_json(json["on_available"]))
	if json.has("on_active"):
		for action in json["on_active"]:
			entry.on_active.push_back(ObjectSerializationRegistry.deserialize_json(json["on_active"]))
	if json.has("on_complete"):
		for action in json["on_complete"]:
			entry.on_complete.push_back(ObjectSerializationRegistry.deserialize_json(json["on_complete"]))
	if json.has("on_fail"):
		for action in json["on_fail"]:
			entry.on_fail.push_back(ObjectSerializationRegistry.deserialize_json(json["on_fail"]))

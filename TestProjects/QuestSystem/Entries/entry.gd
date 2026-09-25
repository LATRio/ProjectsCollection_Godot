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
	match status:
		# TODO: Maybe notify parent on becoming AVAILABLE or INPROGRESS so that
		# GUI can be updated with exclamation mark?
		QuestSystem.EntryStatus.AVAILABLE:
			if not on_available.is_empty():
				for action in on_available:
					action.execute()
		QuestSystem.EntryStatus.INPROGRESS:
			if not on_active.is_empty():
				for action in on_active:
					action.execute()
		QuestSystem.EntryStatus.COMPLETED:
			if not on_complete.is_empty():
				for action in on_complete:
					action.execute()
			if parent_id != -1:
				QuestSystem.get_entry(parent_id).refresh_entry()
		QuestSystem.EntryStatus.FAILED:
			if not on_fail.is_empty():
				for action in on_fail:
					action.execute()
			if parent_id != -1:
				QuestSystem.get_entry(parent_id).refresh_entry()
		_:
			pass
	return true


func get_status() -> QuestSystem.EntryStatus:
	return status


func get_parent_id() -> int:
	return parent_id


func get_next_sibling_entry() -> int:
	return -1


func get_child_entry_ids() -> PackedInt32Array:
	return []


func evaluate_availability() -> bool:
	if not availability_condition or availability_condition.evaluate():
		set_status(QuestSystem.EntryStatus.AVAILABLE)
		return true
	return false


func evaluate_activation() -> bool:
	if not activation_condition or activation_condition.evaluate():
		set_status(QuestSystem.EntryStatus.INPROGRESS)
		return true
	return false


func evaluate_completion() -> bool:
	if completion_condition.evaluate():
		set_status(QuestSystem.EntryStatus.COMPLETED)
		return true
	return false


func evaluate_failure() -> bool:
	if failure_condition.evaluate():
		set_status(QuestSystem.EntryStatus.FAILED)
		return true
	return false


func refresh_entry() -> void:
	match status:
		QuestSystem.EntryStatus.UNKNOWN:
			evaluate_availability()
		QuestSystem.EntryStatus.AVAILABLE:
			evaluate_activation()
		QuestSystem.EntryStatus.INPROGRESS:
			if not evaluate_completion():
				evaluate_failure()
		_:
			pass


static func deserialize_json(entry: Entry, json: Dictionary) -> void:
	if not json.has("id"):
		push_error("[JSONDeserializer] Couldn't find key 'id' in entry JSON: ", json)
		return
	entry.id = json["id"]
	
	if json.has("availability_condition"):
		entry.availability_condition = ObjectSerializationRegistry.deserialize_json(json["availability_condition"])
		if entry.availability_condition:
			entry.availability_condition.caller_id = entry.id
	
	if json.has("activation_condition"):
		entry.activation_condition = ObjectSerializationRegistry.deserialize_json(json["activation_condition"])
		if entry.activation_condition:
			entry.activation_condition.caller_id = entry.id
	
	if json.has("completion_condition"):
		entry.completion_condition = ObjectSerializationRegistry.deserialize_json(json["completion_condition"])
	else:
		entry.completion_condition = AllChildEntriesComplete_Condition.new()
	entry.completion_condition.caller_id = entry.id
	
	if json.has("failure_condition"):
		entry.failure_condition = ObjectSerializationRegistry.deserialize_json(json["failure_condition"])
	else:
		entry.failure_condition = AnyChildEntryFailed_Condition.new()
	entry.failure_condition.caller_id = entry.id
	
	if json.has("on_available"):
		for action in json["on_available"]:
			entry.on_available.push_back(ObjectSerializationRegistry.deserialize_json(json["on_available"]))
			entry.on_available.back().caller_id = entry.id
	
	if json.has("on_active"):
		for action in json["on_active"]:
			entry.on_active.push_back(ObjectSerializationRegistry.deserialize_json(json["on_active"]))
			entry.on_active.back().caller_id = entry.id
	
	if json.has("on_complete"):
		for action in json["on_complete"]:
			entry.on_complete.push_back(ObjectSerializationRegistry.deserialize_json(json["on_complete"]))
			entry.on_complete.back().caller_id = entry.id
	
	if json.has("on_fail"):
		for action in json["on_fail"]:
			entry.on_fail.push_back(ObjectSerializationRegistry.deserialize_json(json["on_fail"]))
			entry.on_fail.back().caller_id = entry.id

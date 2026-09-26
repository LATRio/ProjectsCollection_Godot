extends Node

enum EntryStatus {
	UNKNOWN,
	AVAILABLE,
	INPROGRESS,
	COMPLETED,
	CANCELLED,
	FAILED,
	LOCKED,
	SKIPPED,
	ERROR,
}


# Store everything in Dictionary to eliminate duped IDs
var entries: Dictionary[int, Entry]

# These store actual quest entry objects
var questlines: PackedInt32Array
var quests: PackedInt32Array
var queststeps: PackedInt32Array
# This stores IDs of entries sorted by their status
# Prevents going through completed or failed entries and checking their conditions
# Used for polling conditions of all entries at once
var all_entries_by_status: Dictionary[QuestSystem.EntryStatus, PackedInt32Array] = {
	QuestSystem.EntryStatus.UNKNOWN: [],
	QuestSystem.EntryStatus.AVAILABLE: [],
	QuestSystem.EntryStatus.INPROGRESS: [],
	QuestSystem.EntryStatus.COMPLETED: [],
	QuestSystem.EntryStatus.CANCELLED: [],
	QuestSystem.EntryStatus.FAILED: [],
	QuestSystem.EntryStatus.LOCKED: [],
	QuestSystem.EntryStatus.SKIPPED: [],
	QuestSystem.EntryStatus.ERROR: []
}

# Store independent quests that don't belong in any questline
var free_quests: PackedInt32Array

var _active := false


func _ready() -> void:
	_active = true
	
	var db_file := FileAccess.open("res://TestProjects/QuestSystem/Database/quest_db.json", FileAccess.READ)
	var json := JSON.new()
	var res := json.parse(db_file.get_as_text())
	if res != OK:
		push_error("[QuestSystem] Failed to parse db! Error at line {0}: {1}".format([json.get_error_line(), json.get_error_message()]))
		return
	if typeof(json.data) == TYPE_DICTIONARY:
		if json.data.has("questlines"):
			for questline_json in json.data["questlines"]:
				var questline := ObjectSerializationRegistry.deserialize_json(questline_json)
				add_questline(questline)
		if json.data.has("free_quests"):
			for quest_json in json.data["free_quests"]:
				var quest := ObjectSerializationRegistry.deserialize_json(quest_json)
				add_quest(quest, true)
	print("Finished parsing quest database.")
	
	# TODO: Feels like this will benefit a lot from ECS approach...
	for entry_id in questlines:
		var questline := get_questline(entry_id)
		questline.evaluate_availability()
	for entry_id in free_quests:
		var quest := get_quest(entry_id)
		quest.evaluate_availability()


func entry_exists(entry_id: int) -> bool:
	return entries.has(entry_id)


func is_questline_id(entry_id: int) -> bool:
	if not questlines.has(entry_id):
		push_error("[QuestSystem] Entry ID [{0}] isn't a questline!".format([entry_id]))
		return false
	return true


func is_quest_id(entry_id: int) -> bool:
	if quests.has(entry_id) or free_quests.has(entry_id):
		return true
	push_error("[QuestSystem] Entry ID [{0}] isn't a quest!".format([entry_id]))
	return false


func is_queststep_id(entry_id: int) -> bool:
	if not queststeps.has(entry_id):
		push_error("[QuestSystem] Entry ID [{0}] isn't a quest step!".format([entry_id]))
		return false
	return true


func add_questline(questline: QuestlineEntry) -> void:
	if add_entry(questline):
		questlines.push_back(questline.id)


func add_quest(quest: QuestEntry, is_free: bool = false) -> void:
	if add_entry(quest):
		if is_free:
			free_quests.push_back(quest.id)
		else:
			quests.push_back(quest.id)


func add_queststep(queststep: QuestStepEntry) -> void:
	if add_entry(queststep):
		queststeps.push_back(queststep.id)


func add_entry(entry: Entry) -> bool:
	if entry_exists(entry.id):
		push_error("[QuestSystem] Entry ID [{0}] already exists!".format([entry.id]))
		return false
	else:
		entries[entry.id] = entry
		all_entries_by_status[entry.get_status()].push_back(entry.id)
		return true


func get_entry(entry_id: int) -> Entry:
	if entry_exists(entry_id):
		return entries[entry_id]
	return null


func get_entry_ids_by_status(status: QuestSystem.EntryStatus) -> PackedInt32Array:
	return all_entries_by_status[status]


func get_array_of_entry_ids_sorted_by_status() -> Array[PackedInt32Array]:
	var merged_arr: PackedInt32Array = []
	merged_arr.append_array(all_entries_by_status[QuestSystem.EntryStatus.UNKNOWN])
	merged_arr.append_array(all_entries_by_status[QuestSystem.EntryStatus.AVAILABLE])
	merged_arr.append_array(all_entries_by_status[QuestSystem.EntryStatus.INPROGRESS])
	return merged_arr


func get_nontrackable_entry_ids() -> PackedInt32Array:
	var merged_arr: PackedInt32Array = []
	merged_arr.append_array(all_entries_by_status[QuestSystem.EntryStatus.COMPLETED])
	merged_arr.append_array(all_entries_by_status[QuestSystem.EntryStatus.CANCELLED])
	merged_arr.append_array(all_entries_by_status[QuestSystem.EntryStatus.FAILED])
	merged_arr.append_array(all_entries_by_status[QuestSystem.EntryStatus.LOCKED])
	merged_arr.append_array(all_entries_by_status[QuestSystem.EntryStatus.SKIPPED])
	# skip erroneous entries?
	return merged_arr


func get_questline(entry_id: int) -> QuestlineEntry:
	if entry_exists(entry_id) and is_questline_id(entry_id):
		return entries[entry_id]
	return null


func get_quest(entry_id: int) -> QuestEntry:
	if entry_exists(entry_id):
		if is_quest_id(entry_id):
			return entries[entry_id]
	return null


func get_queststep(entry_id: int) -> QuestStepEntry:
	if entry_exists(entry_id) and is_queststep_id(entry_id):
		return entries[entry_id]
	return null


func set_entry_status(entry_id: int, status: QuestSystem.EntryStatus) -> void:
	if not entry_exists(entry_id):
		push_error("[QuestSystem] Cannot set a status of a non existent Entry ID {0}!".format([entry_id]))
	entries[entry_id].set_status(status)


func get_entry_status(entry_id: int) -> QuestSystem.EntryStatus:
	if entry_exists(entry_id):
		return entries[entry_id].get_status()
	push_error("[QuestSystem] Cannot get a status of a non existent Entry ID {0}!".format([entry_id]))
	return QuestSystem.EntryStatus.ERROR


func sort_entry_by_status(entry_id: int, old_status: QuestSystem.EntryStatus, new_status: QuestSystem.EntryStatus) -> void:
	if not all_entries_by_status[old_status].has(entry_id):
		push_error("[QuestSystem] Entry ID [{0}] wasn't added into a list sorted by status!".format([entry_id]))
	all_entries_by_status[old_status].erase(entry_id)
	all_entries_by_status[new_status].push_back(entry_id)


func status_to_string(status: QuestSystem.EntryStatus) -> String:
	match status:
		QuestSystem.EntryStatus.UNKNOWN:
			return "UNKNOWN"
		QuestSystem.EntryStatus.AVAILABLE:
			return "AVAILABLE"
		QuestSystem.EntryStatus.INPROGRESS:
			return "INPROGRESS"
		QuestSystem.EntryStatus.COMPLETED:
			return "COMPLETED"
		QuestSystem.EntryStatus.CANCELLED:
			return "CANCELLED"
		QuestSystem.EntryStatus.FAILED:
			return "FAILED"
		QuestSystem.EntryStatus.LOCKED:
			return "LOCKED"
		QuestSystem.EntryStatus.SKIPPED:
			return "SKIPPED"
		QuestSystem.EntryStatus.ERROR:
			return "ERROR"
		_:
			return "-1"

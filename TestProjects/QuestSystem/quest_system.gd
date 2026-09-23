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
var questlines: Array[int]
var quests: Array[int]
var queststeps: Array[int]
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
var free_quests: Array[int]

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
				all_entries_by_status[questline.get_status()].push_back(questline.id)
				add_questline(questline)
		if json.data.has("free_quests"):
			for quest_json in json.data["free_quests"]:
				var quest := ObjectSerializationRegistry.deserialize_json(quest_json)
				all_entries_by_status[quest.get_status()].push_back(quest.id)
				add_quest(quest)
	print("Finished parsing quest database.")


func entry_exists(entry_id: int) -> bool:
	return entries.has(entry_id)


func is_questline_id(entry_id: int) -> bool:
	if not questlines.has(entry_id):
		push_error("[QuestSystem] Entry ID [{0}] isn't a questline!".format([entry_id]))
		return false
	return true


func is_quest_id(entry_id: int) -> bool:
	if not quests.has(entry_id):
		push_error("[QuestSystem] Entry ID [{0}] isn't a quest!".format([entry_id]))
		return false
	return true


func is_queststep_id(entry_id: int) -> bool:
	if not queststeps.has(entry_id):
		push_error("[QuestSystem] Entry ID [{0}] isn't a quest step!".format([entry_id]))
		return false
	return true


func add_questline(questline: QuestlineEntry) -> void:
	add_entry(questline)


func add_quest(quest: QuestEntry, is_free: bool = false) -> void:
	if add_entry(quest) and is_free:
		free_quests.push_back(quest.id)


func add_queststep(queststep: QuestStepEntry) -> void:
	add_entry(queststep)


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


func get_questline(entry_id: int) -> QuestlineEntry:
	if entry_exists(entry_id) and is_questline_id(entry_id):
		return entries[entry_id]
	return null


func get_quest(entry_id: int) -> QuestEntry:
	if entry_exists(entry_id) and is_quest_id(entry_id):
		return entries[entry_id]
	return null


func get_queststep(entry_id: int) -> QuestStepEntry:
	if entry_exists(entry_id) and is_queststep_id(entry_id):
		return entries[entry_id]
	return null


func set_entry_status(entry_id: int, status: QuestSystem.EntryStatus) -> void:
	if entry_exists(entry_id):
		entries[entry_id].set_status(status)
	push_error("[QuestSystem] Cannot set a status of a non existent Entry ID {0}!".format([entry_id]))


func get_entry_status(entry_id: int) -> QuestSystem.EntryStatus:
	if entry_exists(entry_id):
		return entries[entry_id].get_status()
	push_error("[QuestSystem] Cannot get a status of a non existent Entry ID {0}!".format([entry_id]))
	return QuestSystem.EntryStatus.ERROR


func sort_entry_by_status(entry_id: int, old_status: QuestSystem.EntryStatus, new_status: QuestSystem.EntryStatus) -> void:
	if all_entries_by_status[old_status].has(entry_id):
		all_entries_by_status[old_status].erase(entry_id)
		all_entries_by_status[new_status].push_back(entry_id)
	push_error("[QuestSystem] Entry ID [{0}] wasn't added into a list sorted by status!".format([entry_id]))

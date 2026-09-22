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
var all_entries_by_status: Dictionary[QuestSystem.EntryStatus, PackedInt32Array]

# Store independent quests that don't belong in any questline
var free_quests: Array[int]

var _active := false


func init() -> void:
	_active = true
	# TODO: actually parse database


func entry_exists(entry_id: int) -> bool:
	if not entries.has(entry_id):
		push_error("[QuestSystem] Entry ID [{0}] doesn't exist in a database!", entry_id)
		return false
	return true


func is_questline_id(entry_id: int) -> bool:
	if not questlines.has(entry_id):
		push_error("[QuestSystem] Entry ID [{0}] isn't a questline!", entry_id)
		return false
	return true


func is_quest_id(entry_id: int) -> bool:
	if not quests.has(entry_id):
		push_error("[QuestSystem] Entry ID [{0}] isn't a quest!", entry_id)
		return false
	return true


func is_queststep_id(entry_id: int) -> bool:
	if not queststeps.has(entry_id):
		push_error("[QuestSystem] Entry ID [{0}] isn't a quest step!", entry_id)
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
		push_error("[QuestSystem] Entry ID [{0}] already exists!", entry.id)
		return false
	else:
		entries[entry.id] = entry
		all_entries_by_status[entry.status].push_back(entry.id)
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
	push_error("[QuestSystem] Cannot set a status of a non existent Entry ID {0}!", entry_id)


func get_entry_status(entry_id: int) -> QuestSystem.EntryStatus:
	if entry_exists(entry_id):
		return entries[entry_id].get_status()
	push_error("[QuestSystem] Cannot get a status of a non existent Entry ID {0}!", entry_id)
	return QuestSystem.EntryStatus.ERROR


func sort_entry_by_status(entry_id: int, old_status: QuestSystem.EntryStatus, new_status: QuestSystem.EntryStatus) -> void:
	if all_entries_by_status[old_status].has(entry_id):
		all_entries_by_status[old_status].erase(entry_id)
		all_entries_by_status[new_status].push_back(entry_id)
	push_error("[QuestSystem] Entry ID [{0}] wasn't added into a list sorted by status!", entry_id)

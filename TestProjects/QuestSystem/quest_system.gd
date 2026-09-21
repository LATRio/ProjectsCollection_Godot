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
}

# Store everything in Dictionary to eliminate duped IDs

var questlines: Dictionary[int, QuestlineEntry]
var free_quests: Dictionary[int, QuestEntry]

var _all_quests: Dictionary[int, QuestEntry]
var _all_queststeps: Dictionary[int, QuestStepEntry]

var _active := false


func init() -> void:
	_active = true
	# parse json database
	# TODO: test uniqueness of entries IDs regardless or their type


func set_questline_status(questline_id: int, status: QuestSystem.EntryStatus) -> void:
	if questlines.has(questline_id):
		questlines[questline_id].set_status(status)


func set_quest_status(quest_id: int, status: QuestSystem.EntryStatus) -> void:
	if _all_quests.has(quest_id):
		_all_quests[quest_id].set_status(status)


func set_queststep_status(queststep_id: int, status: QuestSystem.EntryStatus) -> void:
	if _all_queststeps.has(queststep_id):
		_all_queststeps[queststep_id].set_status(status)

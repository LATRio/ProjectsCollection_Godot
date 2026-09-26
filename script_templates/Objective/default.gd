class_name _CLASS_
extends _BASE_

static var type := "_CLASS_"


func register_tracker() -> void:
	pass


func unregister_tracker() -> void:
	pass


func _on_objective_updated(_args: Variant) -> void:
	pass


static func deserialize(_json: Dictionary) -> _CLASS_:
	var _CLASS_SNAKE_CASE_ := _CLASS_.new()
	return _CLASS_SNAKE_CASE_


#func _on_objective_completed() -> void:
#	print("[QuestSystem] Objective of QuestStep ID [{0}] was completed!".format([parent_step_id]))
#	QuestSystem.get_entry(parent_step_id).refresh_entry()
#	unregister_tracker()


#func _on_objective_failed() -> void:
#	print("[QuestSystem] Objective of QuestStep ID [{0}] was failed!".format([parent_step_id]))
#	QuestSystem.get_entry(parent_step_id).refresh_entry()
#	unregister_tracker()

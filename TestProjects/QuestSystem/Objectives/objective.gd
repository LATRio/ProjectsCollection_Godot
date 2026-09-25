class_name Objective
extends RefCounted

var parent_step_id: int


# Completion or failure behavior is handled by QuestStep.
# This 'Objective' only responsible for tracking if objective
# is completed or failed.

func register_tracker() -> void:
	# Example: must save NPC
	# NPCs.objectives_target.on_saved.connect(_on_objective_completed)
	# NPCs.objectives_target.on_died.connect(_on_objective_failed)
	pass

# Array accepts any Variant compatible value
func _on_objective_updated(_args: Array) -> void:
	# current_item_count += args[0] as int
	# if current_item_count >= target_item_count:
	# 	_on_objective_completed()
	# Failure condition for this type of objectives is handled by parent QuestStep.
	# OR
	# If objective is tracking amount of allies that fell during battle:
	# if allied_fell_count >= allied_fell_limit:
	# 	_on_objective_failed()
	# Completion condition in this case is handled by parent QuestStep as well.
	pass


func _on_objective_completed() -> void:
	print("[QuestSystem] Objective of QuestStep ID [{0}] was completed!".format([parent_step_id]))
	QuestSystem.get_entry(parent_step_id).set_status(QuestSystem.EntryStatus.COMPLETED)
	unregister_tracker()


func _on_objective_failed() -> void:
	print("[QuestSystem] Objective of QuestStep ID [{0}] was completed!".format([parent_step_id]))
	QuestSystem.get_entry(parent_step_id).set_status(QuestSystem.EntryStatus.FAILED)
	unregister_tracker()


func unregister_tracker() -> void:
	# NPCs.objectives_target.on_saved.disconnect(_on_objective_completed)
	# NPCs.objectives_target.on_died.disconnect(_on_objective_failed)
	pass

class_name ObjectiveCompleted_Condition
extends Condition

static func _get_type() -> String:
	return "ObjectiveCompleted"


func _evaluate() -> bool:
	var queststep := QuestSystem.get_queststep(caller_id)
	if not queststep:
		return false
	return queststep.objective.is_completed


static func _deserialize(_json: Dictionary) -> ObjectiveCompleted_Condition:
	return ObjectiveCompleted_Condition.new()

class_name ObjectiveFailed_Condition
extends Condition

static func _get_type() -> String:
	return "ObjectiveFailed"


func _evaluate() -> bool:
	var queststep := QuestSystem.get_queststep(caller_id)
	if not queststep:
		return false
	return queststep.objective.is_failed


static func _deserialize(_json: Dictionary) -> ObjectiveFailed_Condition:
	return ObjectiveFailed_Condition.new()

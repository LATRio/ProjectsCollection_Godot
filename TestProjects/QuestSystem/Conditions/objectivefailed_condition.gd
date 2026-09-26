class_name ObjectiveFailed_Condition
extends Condition

static var type := "ObjectiveFailed"


func evaluate() -> bool:
	var queststep := QuestSystem.get_queststep(caller_id)
	return queststep.objective.is_failed


static func deserialize(_json: Dictionary) -> ObjectiveFailed_Condition:
	return ObjectiveFailed_Condition.new()

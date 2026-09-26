class_name ObjectiveCompleted_Condition
extends Condition

static var type := "ObjectiveCompleted"


func evaluate() -> bool:
	var queststep := QuestSystem.get_queststep(caller_id)
	return queststep.objective.is_completed


static func deserialize(_json: Dictionary) -> ObjectiveCompleted_Condition:
	return ObjectiveCompleted_Condition.new()

class_name ObjectiveCompleted_Condition
extends Condition

static var type := "ObjectiveCompleted"


func evaluate() -> bool:
	return false


static func deserialize(_json: Dictionary) -> ObjectiveCompleted_Condition:
	return ObjectiveCompleted_Condition.new()

class_name True_Condition
extends Condition

static var type := "True"


func evaluate() -> bool:
	return true


static func deserialize(_json: Dictionary) -> True_Condition:
	return True_Condition.new()

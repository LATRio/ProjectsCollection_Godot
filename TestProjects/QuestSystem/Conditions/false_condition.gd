class_name False_Condition
extends Condition

static var type := "False"


func evaluate() -> bool:
	return false


static func deserialize(_json: Dictionary) -> False_Condition:
	return False_Condition.new()

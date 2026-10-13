class_name False_Condition
extends Condition

static func _get_type() -> String:
	return "False"


func _evaluate() -> bool:
	return false


static func _deserialize(_json: Dictionary) -> False_Condition:
	return False_Condition.new()

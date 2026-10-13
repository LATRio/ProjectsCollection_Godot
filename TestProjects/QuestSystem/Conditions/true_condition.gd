class_name True_Condition
extends Condition

static func _get_type() -> String:
	return "True"


func _evaluate() -> bool:
	return true


static func _deserialize(_json: Dictionary) -> True_Condition:
	return True_Condition.new()

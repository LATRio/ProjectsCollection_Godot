class_name Comparison_Condition
extends Condition

enum CompareOp {
	LESS,
	GREATER,
	EQUAL,
	NOT_EQUAL,
	LESS_OR_EQUAL,
	GREATER_OR_EQUAL,
}

var compare_op: CompareOp
var lhs: Variant
var rhs: Variant


func evaluate() -> bool:
	var lhs_value = lhs
	var rhs_value = rhs
	
	match compare_op:
		CompareOp.LESS:
			return lhs_value < rhs_value
		CompareOp.GREATER:
			return lhs_value > rhs_value
		CompareOp.EQUAL:
			return lhs_value == rhs_value
		CompareOp.NOT_EQUAL:
			return lhs_value != rhs_value
		CompareOp.LESS_OR_EQUAL:
			return lhs_value <= rhs_value
		CompareOp.GREATER_OR_EQUAL:
			return lhs_value >= rhs_value
		_:
			return false


func get_value_from_variant(value: Variant) -> Variant:
	if typeof(value) == Variant.Type.TYPE_INT or typeof(value) == Variant.Type.TYPE_FLOAT:
		return value
	elif typeof(value) == Variant.Type.TYPE_STRING:
		# TODO: turn "value" into actual numerical value
		return float(value)
	printerr("Value passed to lhs or rhs of comparison condition has invalid type!")
	return 0

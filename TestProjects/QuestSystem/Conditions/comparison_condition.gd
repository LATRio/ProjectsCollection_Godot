class_name Comparison_Condition
extends Condition

static var type := "comparison"

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


func activate_tracking() -> void:
	pass


func deactivate_tracking() -> void:
	pass


static func deserialize(json: Dictionary) -> Comparison_Condition:
	if not json.has("compare_op"):
		push_error("[JSONDeserializer] Comparison_Condition json entry doesn't have 'compare_op' key.")
		return null
	if not json.has("lhs"):
		push_error("[JSONDeserializer] Comparison_Condition json entry doesn't have 'lhs' key.")
		return null
	if not json.has("rhs"):
		push_error("[JSONDeserializer] Comparison_Condition json entry doesn't have 'rhs' key.")
		return null
	var comparison := Comparison_Condition.new()
	comparison.compare_op = json["compare_op"]
	comparison.lhs = json["lhs"]
	comparison.rhs = json["rhs"]
	
	#comparison.is_tracking = true
	#comparison.activate_tracking()
	
	return comparison

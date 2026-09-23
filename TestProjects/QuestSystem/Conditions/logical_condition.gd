class_name Logical_Condition
extends Condition

static var type := "logical"

enum LogicalOp {
	AND,
	OR,
	NOT,
	NAND,
	NOR,
	XOR,
}

var logical_op: LogicalOp
var inputs: Array[Condition]


func evaluate() -> bool:
	if inputs.is_empty():
		push_error("[QuestSystem] 'inputs' field of logical condition cannot be empty!")
		return false
	
	match logical_op:
		LogicalOp.AND:
			return evaluate_AND()
		LogicalOp.OR:
			return evaluate_OR()
		LogicalOp.NOT:
			return evaluate_NOT()
		LogicalOp.NAND:
			return evaluate_NAND()
		LogicalOp.NOR:
			return evaluate_NOR()
		LogicalOp.XOR:
			return evaluate_XOR()
		_:
			return false


func evaluate_AND() -> bool:
	var ret := true
	for input in inputs:
		ret = ret and input.evaluate()
	return ret


func evaluate_OR() -> bool:
	var ret := false
	for input in inputs:
		ret = ret or input.evaluate()
	return ret


func evaluate_NOT() -> bool:
	if inputs.size() > 1:
		printerr("[QuestSystem] NOT logical operation only accepts 1 input!")
		return false
	
	return not inputs.front().evaluate()


func evaluate_NAND() -> bool:
	return not evaluate_AND()


func evaluate_NOR() -> bool:
	return not evaluate_NOR()


func evaluate_XOR() -> bool:
	var true_count := 0
	for input in inputs:
		if input.evaluate():
			true_count += 1
	return true_count % 2 == 1


static func deserialize(json: Dictionary) -> Logical_Condition:
	if not json.has("logical_op"):
		push_error("[JSONDeserializer] Logical_Condition json entry doesn't have 'logical_op' key.")
		return null
	if not json.has("inputs"):
		push_error("[JSONDeserializer] Logical_Condition json entry doesn't have 'inputs' key.")
		return null
	var logical := Logical_Condition.new()
	logical.logical_op = json["logical_op"]
	logical.inputs = json["inputs"]
	return logical

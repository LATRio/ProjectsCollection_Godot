#include "logical_condition.hpp"

#include "object_serialization_registry/object_serialization_registry.hpp"

namespace QS {
void Logical_Condition::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_get_type"), &Logical_Condition::_get_type);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_deserialize", "json"), &Logical_Condition::_deserialize);
}

bool Logical_Condition::_evaluate_impl() {
	ERR_FAIL_COND_V_MSG(m_op == LogicalOP::INVALID, false, "[Logical_Condition] 'm_op' is invalid.");
	ERR_FAIL_COND_V_MSG(m_inputs.is_empty(), false, "[Logical_Condition] 'm_inputs' is empty.");

	switch (m_op) {
		case LogicalOP::AND:
			return evaluate_AND();
		case LogicalOP::OR:
			return evaluate_OR();
		case LogicalOP::NOT:
			return evaluate_NOT();
		case LogicalOP::NAND:
			return evaluate_NAND();
		case LogicalOP::NOR:
			return evaluate_NOR();
		case LogicalOP::XOR:
			return evaluate_XOR();
		default:
			ERR_FAIL_V_MSG(false, String("[Logical_Condition] 'm_op' is some unknown value: ").format(Array::make(static_cast<int32_t>(m_op))));
	}
}

Ref<Logical_Condition> Logical_Condition::_deserialize(const Dictionary &p_json) {
	ERR_FAIL_COND_V_MSG(p_json.has("op"), nullptr, String("[Logical_Condition] Failed to find 'op' in json: {0}").format(Array::make(p_json)));
	ERR_FAIL_COND_V_MSG(p_json.has("inputs"), nullptr, String("[Logical_Condition] Failed to find 'op' in json: {0}").format(Array::make(p_json)));
	Ref logical = memnew(Logical_Condition);
	logical->m_op = _string_to_logical_op(p_json["op"]);
	Array inputs = p_json["inputs"];
	if (logical->m_op == LogicalOP::NOT) {
		ERR_FAIL_COND_V_MSG(inputs.size() != 1, nullptr, String("[Logical_Condition] 'NOT' shouldn't have more than one input: {0}").format(Array::make(p_json)));
	}
	for (const auto &input_json : inputs) {
		Ref<Condition> condition = ObjectSerializationRegistry::get_singleton()->deserialize_json(input_json);
		ERR_FAIL_NULL_V_MSG(condition, nullptr, String("[Logical_Condition] Failed to parse 'inputs' entry: ").format(Array::make(input_json)));
		logical->m_inputs.push_back(condition);
	}
	return logical;
}

Logical_Condition::LogicalOP Logical_Condition::_string_to_logical_op(const String &p_str_op) {
	String low_op = p_str_op.to_lower();
	if (low_op == "and") {
		return LogicalOP::AND;
	}
	if (low_op == "or") {
		return LogicalOP::OR;
	}
	if (low_op == "not") {
		return LogicalOP::NOT;
	}
	if (low_op == "nand") {
		return LogicalOP::NAND;
	}
	if (low_op == "nor") {
		return LogicalOP::NOR;
	}
	if (low_op == "xor") {
		return LogicalOP::XOR;
	}
	ERR_FAIL_V_MSG(LogicalOP::INVALID, "[Logical_Condition] 'op' value is invalid: " + p_str_op);
}

bool Logical_Condition::evaluate_AND() const {
	bool ret{};
	for (const auto &input : m_inputs) {
		Ref<Condition> cond = input;
		ret &= cond->evaluate();
	}
	return ret;
}

bool Logical_Condition::evaluate_OR() const {
	bool ret{};
	for (const auto &input : m_inputs) {
		Ref<Condition> cond = input;
		ret &= cond->evaluate();
	}
	return ret;
}

bool Logical_Condition::evaluate_NOT() const {
	Ref<Condition> cond = m_inputs.back();
	return cond->evaluate();
}

bool Logical_Condition::evaluate_NAND() const {
	return !evaluate_AND();
}

bool Logical_Condition::evaluate_NOR() const {
	return !evaluate_OR();
}

bool Logical_Condition::evaluate_XOR() const {
	int true_count{};
	for (const auto &input : m_inputs) {
		Ref<Condition> cond = input;
		if (cond->evaluate()) {
			true_count++;
		}
	}
	return true_count % 2 == 1;
}
}

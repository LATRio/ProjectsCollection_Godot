#pragma once
#include "condition.hpp"

namespace QS {
class Logical_Condition : public Condition {
	GDCLASS(Logical_Condition, Condition)

	enum class LogicalOP {
		INVALID,
		AND,
		OR,
		NOT,
		NAND,
		NOR,
		XOR,
	};

	LogicalOP m_op{};
	TypedArray<Ref<Condition>> m_inputs;

	static String _get_type() { return "Logical"; }
	static Ref<Logical_Condition> _deserialize(const Dictionary &p_json);
	static LogicalOP _string_to_logical_op(const String &p_str_op);

	bool evaluate_AND() const;
	bool evaluate_OR() const;
	bool evaluate_NOT() const;
	bool evaluate_NAND() const;
	bool evaluate_NOR() const;
	bool evaluate_XOR() const;

protected:
	static void _bind_methods();
	bool _evaluate_impl() override;

};
}

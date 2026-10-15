#pragma once
#include "condition.hpp"

namespace QS {
class False_Condition : public Condition {
	GDCLASS(False_Condition, Condition)

	static String _get_type() { return "False"; }
	static Ref<False_Condition> _deserialize(const Dictionary &p_json);

protected:
	static void _bind_methods();
	bool _evaluate_impl() override;

};
}

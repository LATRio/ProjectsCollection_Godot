#pragma once
#include "condition.hpp"

namespace QS {
class True_Condition : public Condition {
	GDCLASS(True_Condition, Condition)

	static String _get_type() { return "True"; }
	static Ref<True_Condition> _deserialize(const Dictionary &p_json);

protected:
	static void _bind_methods();
	bool _evaluate_impl() override;

};
}

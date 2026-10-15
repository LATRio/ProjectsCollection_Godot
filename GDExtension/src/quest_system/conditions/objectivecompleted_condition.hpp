#pragma once
#include "condition.hpp"

namespace QS {
class ObjectiveCompleted_Condition : public Condition {
	GDCLASS(ObjectiveCompleted_Condition, Condition)

	static String _get_type() { return "ObjectiveCompleted"; }
	static Ref<ObjectiveCompleted_Condition> _deserialize(const Dictionary &p_json);

protected:
	static void _bind_methods();
	bool _evaluate_impl() override;

};
}

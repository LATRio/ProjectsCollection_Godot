#pragma once
#include "conditions/condition.hpp"

using namespace godot;

namespace QS {
class ObjectiveFailedCondition : public Condition {
	GDCLASS(ObjectiveFailedCondition, Condition);

	static String _get_type() { return "ObjectiveFailed"; }
	static Ref<ObjectiveFailedCondition> _deserialize(const Dictionary &p_json);

protected:
	static void _bind_methods();
	bool _evaluate_impl() override;

};
}

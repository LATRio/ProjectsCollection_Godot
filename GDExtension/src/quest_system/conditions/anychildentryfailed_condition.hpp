#pragma once
#include "condition.hpp"

namespace QS {
class AnyChildEntryFailed_Condition : public Condition {
	GDCLASS(AnyChildEntryFailed_Condition, Condition)

	static String _get_type() { return "AnyChildEntryFailed"; }
	static Ref<AnyChildEntryFailed_Condition> _deserialize(const Dictionary& p_json);

protected:
	static void _bind_methods();
	bool _evaluate_impl() override;

};
}

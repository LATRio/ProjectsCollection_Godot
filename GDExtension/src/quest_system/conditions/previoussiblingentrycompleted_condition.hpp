#pragma once
#include "condition.hpp"

namespace QS {
class PreviousSiblingEntryCompleted_Condition : public Condition {
	GDCLASS(PreviousSiblingEntryCompleted_Condition, Condition)

	static String _get_type() { return "PreviousSiblingEntryCompleted"; }
	static Ref<PreviousSiblingEntryCompleted_Condition> _deserialize(const Dictionary &p_json);

protected:
	static void _bind_methods();
	bool _evaluate_impl() override;

};
}

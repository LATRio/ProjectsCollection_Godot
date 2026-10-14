#pragma once
#include "condition.hpp"

namespace QS {
class AllChildEntriesCompleted_Condition : public Condition {
	GDCLASS(AllChildEntriesCompleted_Condition, Condition)

	static String _get_type() { return "AllChildEntriesCompleted"; }
	static Ref<AllChildEntriesCompleted_Condition> _deserialize(const Dictionary& p_json);

protected:
	static void _bind_methods();
	bool _evaluate_impl() override;

};
}

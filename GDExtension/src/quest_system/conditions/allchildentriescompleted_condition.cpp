#include "allchildentriescompleted_condition.hpp"

#include "entries/entry.hpp"

namespace QS {
void AllChildEntriesCompleted_Condition::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_get_type"), &AllChildEntriesCompleted_Condition::_get_type);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_deserialize", "json"), &AllChildEntriesCompleted_Condition::_deserialize);
}

bool AllChildEntriesCompleted_Condition::_evaluate_impl() {
	const Ref<Entry> caller = QuestSystem::get_singleton()->get_entry(m_caller_id);
	ERR_FAIL_NULL_V_MSG(caller, false, "[AllChildEntriesCompleted_Condition] Failed to retrieve caller entry " + m_caller_id);
	for (const auto &child_id : caller->get_child_entry_ids()) {
		const Ref<Entry> child = QuestSystem::get_singleton()->get_entry(child_id);
		ERR_FAIL_NULL_V_MSG(caller, false, "[AllChildEntriesCompleted_Condition] Failed to retrieve child entry " + static_cast<StringName>(child_id));
		if (child->get_status() != ENTRY_STATUS_COMPLETED) {
			return false;
		}
	}
	return true;
}

Ref<AllChildEntriesCompleted_Condition> AllChildEntriesCompleted_Condition::_deserialize(const Dictionary &p_json) {
	return memnew(AllChildEntriesCompleted_Condition);
}
}

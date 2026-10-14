#include "anychildentryfailed_condition.hpp"

#include "entries/entry.hpp"

namespace QS {
void AnyChildEntryFailed_Condition::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_get_type"), &AnyChildEntryFailed_Condition::_get_type);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_deserialize", "json"), &AnyChildEntryFailed_Condition::_deserialize);
}

bool AnyChildEntryFailed_Condition::_evaluate_impl() {
	const Ref<Entry> caller = QuestSystem::get_singleton()->get_entry(m_caller_id);
	ERR_FAIL_NULL_V_MSG(caller, false, "[AnyChildEntryFailed_Condition] Failed to retrieve caller entry " + m_caller_id);
	for (const auto &child_id : caller->get_child_entry_ids()) {
		const Ref<Entry> child = QuestSystem::get_singleton()->get_entry(child_id);
		ERR_FAIL_NULL_V_MSG(caller, false, "[AnyChildEntryFailed_Condition] Failed to retrieve child entry " + static_cast<StringName>(child_id));
		if (child->get_status() == ENTRY_STATUS_FAILED) {
			return true;
		}
	}
	return false;
}

Ref<AnyChildEntryFailed_Condition> AnyChildEntryFailed_Condition::_deserialize(const Dictionary &p_json) {
	return memnew(AnyChildEntryFailed_Condition);
}
}

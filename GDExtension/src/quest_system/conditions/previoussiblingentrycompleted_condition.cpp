#include "previoussiblingentrycompleted_condition.hpp"

#include "entries/entry.hpp"

namespace QS {
void PreviousSiblingEntryCompleted_Condition::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_get_type"), &PreviousSiblingEntryCompleted_Condition::_get_type);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_deserialize", "json"), &PreviousSiblingEntryCompleted_Condition::_deserialize);
}

bool PreviousSiblingEntryCompleted_Condition::_evaluate_impl() {
	const Ref<Entry> caller = QuestSystem::get_singleton()->get_entry(m_caller_id);
	ERR_FAIL_NULL_V_MSG(caller, false, "[PreviousSiblingEntryCompleted_Condition] Failed to retrieve caller entry " + m_caller_id);
	const StringName sibling_id = caller->get_previous_sibling_entry_id();
	ERR_FAIL_COND_V_MSG(sibling_id.is_empty(), false, "[PreviousSiblingEntryCompleted_Condition] Failed to retrieve pervious sibling entry id of " + m_caller_id);
	const Ref<Entry> sibling_entry = QuestSystem::get_singleton()->get_entry(sibling_id);
	ERR_FAIL_NULL_V_MSG(sibling_entry, false, "[PreviousSiblingEntryCompleted_Condition] Failed to retrieve sibling entry " + sibling_id);
	return sibling_entry->get_status() == ENTRY_STATUS_COMPLETED;
}

Ref<PreviousSiblingEntryCompleted_Condition> PreviousSiblingEntryCompleted_Condition::_deserialize(const Dictionary &p_json) {
	return memnew(PreviousSiblingEntryCompleted_Condition);
}
}

#include "objectivecompleted_condition.hpp"

#include "entries/quest_step_entry.hpp"

namespace QS {
void ObjectiveCompleted_Condition::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_get_type"), &ObjectiveCompleted_Condition::_get_type);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_deserialize", "json"), &ObjectiveCompleted_Condition::_deserialize);
}

bool ObjectiveCompleted_Condition::_evaluate_impl() {
	Ref queststep = QuestSystem::get_singleton()->get_queststep(m_caller_id);
	ERR_FAIL_NULL_V_MSG(queststep, false, "[ObjectiveCompleted_Condition] Failed to retrieve caller " + m_caller_id);
	return queststep->get_objective()->get_is_completed();
}

Ref<ObjectiveCompleted_Condition> ObjectiveCompleted_Condition::_deserialize(const Dictionary &p_json) {
	return memnew(ObjectiveCompleted_Condition);
}
}

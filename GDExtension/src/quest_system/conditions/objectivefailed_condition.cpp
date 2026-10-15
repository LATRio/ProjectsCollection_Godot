#include "objectivefailed_condition.hpp"

#include "entries/quest_step_entry.hpp"

namespace QS {
void ObjectiveFailedCondition::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_get_type"), &ObjectiveFailedCondition::_get_type);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_deserialize", "json"), &ObjectiveFailedCondition::_deserialize);
}

bool ObjectiveFailedCondition::_evaluate_impl() {
	Ref queststep = QuestSystem::get_singleton()->get_queststep(m_caller_id);
	ERR_FAIL_NULL_V_MSG(queststep, false, "[ObjectiveCompleted_Condition] Failed to retrieve caller " + m_caller_id);
	return queststep->get_objective()->get_is_failed();
}

Ref<ObjectiveFailedCondition> ObjectiveFailedCondition::_deserialize(const Dictionary &p_json) {
	return memnew(ObjectiveFailedCondition);
}
}

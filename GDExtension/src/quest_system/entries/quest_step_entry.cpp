#include "quest_step_entry.hpp"

#include "object_serialization_registry/object_serialization_registry.hpp"
#include "objectives/objective.hpp"
#include "quest_entry.hpp"
#include "quest_system.hpp"

namespace QS {
void QuestStepEntry::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_get_type"), &QuestStepEntry::_get_type);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_deserialize", "p_json"), &QuestStepEntry::_deserialize);

	ClassDB::bind_method(D_METHOD("set_objective", "objective"), &QuestStepEntry::set_objective);
	ClassDB::bind_method(D_METHOD("get_objective"), &QuestStepEntry::get_objective);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "objective"), "set_objective", "get_objective");
}

bool QuestStepEntry::_set_status_impl(const EntryStatus p_new_status) {
	//UtilityFunctions::print("QuestStepEntry::_set_status_impl: ", QuestSystem::status_to_string(p_new_status));
	if (p_new_status == ENTRY_STATUS_INPROGRESS) {
		m_objective->register_tracker();
	} else if (p_new_status == ENTRY_STATUS_COMPLETED) {
		m_objective->unregister_tracker();
	}
	return true;
}

StringName QuestStepEntry::_get_previous_sibling_entry_id_impl() const {
	Ref quest = QuestSystem::get_singleton()->get_quest(m_parent_id);
	ERR_FAIL_COND_V_MSG(quest.is_null(), "", String("Failed to retrieve a parent QuestEntry [{0}].").format(Array::make(m_parent_id)));

	auto steps = quest->get_child_entry_ids();
	if (steps.size() < 2) { return ""; }
	for (int i = 1; i < steps.size(); ++i) {
		if (steps[i] == m_id) {
			return steps[i - 1];
		}
	}
	return "";
}

StringName QuestStepEntry::_get_next_sibling_entry_id_impl() const {
	Ref quest = QuestSystem::get_singleton()->get_quest(m_parent_id);
	ERR_FAIL_COND_V_MSG(quest.is_null(), "", String("Failed to retrieve a parent QuestEntry [{0}].").format(Array::make(m_parent_id)));

	auto steps = quest->get_child_entry_ids();
	if (steps.size() < 2) { return ""; }
	for (int i = 0; i < steps.size() - 1; ++i) {
		if (steps[i] == m_id) {
			return steps[i + 1];
		}
	}
	return "";
}

Ref<QuestStepEntry> QuestStepEntry::_deserialize(const Dictionary &p_json) {
	ERR_FAIL_COND_V_MSG(!p_json.has("objective"), nullptr, "[QuestStepEntry::deserialize] Failed to find 'objective' key.");
	ERR_FAIL_COND_V_MSG(p_json["objective"].get_type() != Variant::DICTIONARY, nullptr, "[QuestStepEntry::deserialize] 'objective' is not a dictionary.");
	Ref entry{ memnew(QuestStepEntry) };

	deserialize_json(entry, p_json);

	entry->m_objective = ObjectSerializationRegistry::get_singleton()->deserialize_json(p_json["objective"]);
	ERR_FAIL_COND_V_MSG(entry->m_objective.is_null(), nullptr, "[QuestStepEntry::deserialize] Failed to deserialize an objective.");

	entry->m_objective->set_parent_step_id(entry->m_id);
	return entry;
}
}

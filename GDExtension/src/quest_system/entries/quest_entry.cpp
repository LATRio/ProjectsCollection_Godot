#include "quest_entry.hpp"

#include "object_serialization_registry/object_serialization_registry.hpp"
#include "quest_step_entry.hpp"
#include "quest_system.hpp"

void QS::QuestEntry::_bind_methods() {
	ClassDB::bind_static_method("QuestEntry", D_METHOD("_get_type"), &QuestEntry::_get_type);
	ClassDB::bind_static_method("QuestEntry", D_METHOD("_deserialize", "p_json"), &QuestEntry::_deserialize);
}

Ref<QS::QuestEntry> QS::QuestEntry::_deserialize(const Dictionary &p_json) {
	ERR_FAIL_COND_V_MSG(!p_json.has("steps"), nullptr, "[QuestEntry::deserialize] Failed to find 'steps' key.");
	ERR_FAIL_COND_V_MSG(p_json["steps"].get_type() != Variant::ARRAY, nullptr, "[QuestEntry::deserialize] 'steps' is not an array.");
	Ref entry{memnew(QuestEntry)};

	Entry::_deserialize(entry, p_json);

	Array steps = p_json["steps"];
	if (steps.is_empty()) {
		QuestSystem::printerr("'steps' of Quest ID [{0}] is empty.", entry->m_id);
		return nullptr;
	}
	UtilityFunctions::print("QuestEntry::_deserialize: ", steps);
	for (const auto &step_json : steps) {
		if (step_json.get_type() != Variant::DICTIONARY) {
			QuestSystem::printerr("Quest ID [{0}] must only contain dictionary values in 'steps'!", entry->m_id);
			return nullptr;
		}
		Ref<QuestStepEntry> step_obj{ObjectSerializationRegistry::get_singleton()->deserialize_json(step_json)};
		if (!step_obj.is_valid()) {
			QuestSystem::printerr("Dictionary in 'steps' of Quest ID [{0}] is invalid.", entry->m_id);
			return nullptr;
		}
		step_obj->set_parent_id(entry->m_id);
		entry->m_steps.push_back(step_obj->get_id());
		QuestSystem::get_singleton()->add_queststep(step_obj);
	}
	return entry;
}

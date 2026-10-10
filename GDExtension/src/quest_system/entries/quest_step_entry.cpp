#include "quest_step_entry.hpp"

#include "object_serialization_registry/object_serialization_registry.hpp"
#include "objectives/objective.hpp"
#include "quest_system.hpp"

namespace QS {

void QuestStepEntry::_bind_methods() {
}

Ref<QuestStepEntry> QuestStepEntry::_deserialize(const Dictionary &p_json) {
	ERR_FAIL_COND_V_MSG(!p_json.has("objective"), nullptr, "[QuestStepEntry::deserialize] Failed to find 'objective' key.");
	ERR_FAIL_COND_V_MSG(p_json["objective"].get_type() != Variant::DICTIONARY, nullptr, "[QuestlineEntry::deserialize] 'objective' is not a dictionary.");
	Ref entry{memnew(QuestStepEntry)};

	Entry::_deserialize(entry, p_json);

	entry->m_objective = ObjectSerializationRegistry::get_singleton()->deserialize_json(p_json["objective"]);

	entry->m_objective->set_parent_step_id(entry->m_id);
	return entry;
}

}

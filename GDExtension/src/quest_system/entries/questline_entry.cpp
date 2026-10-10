#include "questline_entry.hpp"

#include "object_serialization_registry/object_serialization_registry.hpp"
#include "quest_entry.hpp"
#include "quest_system.hpp"

namespace QS {

void QuestlineEntry::_bind_methods() {
	ClassDB::bind_static_method("QuestlineEntry", D_METHOD("_get_type"), &QuestlineEntry::_get_type);
	ClassDB::bind_static_method("QuestlineEntry", D_METHOD("_deserialize", "p_json"), &QuestlineEntry::_deserialize);
}

Ref<QuestlineEntry> QuestlineEntry::_deserialize(const Dictionary &p_json) {
	ERR_FAIL_COND_V_MSG(!p_json.has("quests"), nullptr, "[QuestlineEntry::deserialize] Failed to find 'quests' key.");
	ERR_FAIL_COND_V_MSG(p_json["quests"].get_type() != Variant::ARRAY, nullptr, "[QuestlineEntry::deserialize] 'quests' is not an array.");
	Ref entry{memnew(QuestlineEntry)};

	Entry::_deserialize(entry, p_json);

	Array quests = p_json["quests"];
	if (quests.is_empty()) {
		QuestSystem::printerr("'quests' of Questline ID [{0}] is empty.", entry->m_id);
		return nullptr;
	}
	for (const auto &quest_json : quests) {
		if (quest_json.get_type() != Variant::DICTIONARY) {
			QuestSystem::printerr("Questline ID [{0}] must only contain dictionary values in 'quests'!", entry->m_id);
			return nullptr;
		}
		Ref<QuestEntry> quest_obj = ObjectSerializationRegistry::get_singleton()->deserialize_json(quest_json);
		if (!quest_obj.is_valid()) {
			QuestSystem::printerr("Dictionary in 'quests' of Questline ID [{0}] is invalid.", entry->m_id);
			return nullptr;
		}
		quest_obj->set_parent_id(entry->m_id);
		entry->m_quests.push_back(quest_obj->get_id());
		QuestSystem::get_singleton()->add_quest(quest_obj);
	}
	return entry;
}

}

#include "quest_entry.hpp"

#include "object_serialization_registry/object_serialization_registry.hpp"
#include "quest_step_entry.hpp"
#include "quest_system.hpp"
#include "questline_entry.hpp"

void QS::QuestEntry::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_get_type"), &QuestEntry::_get_type);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_deserialize", "p_json"), &QuestEntry::_deserialize);
}

bool QS::QuestEntry::_set_status_impl(const EntryStatus p_new_status) {
	if (p_new_status >= ENTRY_STATUS_COMPLETED) {
		for (const auto& step_id : m_steps) {
			if (QuestSystem::get_singleton()->get_entry_status(step_id) < ENTRY_STATUS_COMPLETED) {
				QuestSystem::get_singleton()->set_entry_status(step_id, ENTRY_STATUS_SKIPPED);
			}
		}
	}
	return true;
}

StringName QS::QuestEntry::_get_previous_sibling_entry_id_impl() const {
	Ref questline = QuestSystem::get_singleton()->get_questline(m_parent_id);
	ERR_FAIL_COND_V_MSG(questline.is_null(), "", String("Failed to retrieve a parent QuestlineEntry [{0}].").format(Array::make(m_parent_id)));

	auto quests = questline->get_child_entry_ids();
	if (quests.size() < 2) { return ""; }
	for (int i = 1; i < quests.size(); ++i) {
		if (quests[i] == m_id) {
			return quests[i - 1];
		}
	}
	return "";
}

StringName QS::QuestEntry::_get_next_sibling_entry_id_impl() const {
	Ref questline = QuestSystem::get_singleton()->get_questline(m_parent_id);
	ERR_FAIL_COND_V_MSG(questline.is_null(), "", String("Failed to retrieve a parent QuestlineEntry [{0}].").format(Array::make(m_parent_id)));

	auto quests = questline->get_child_entry_ids();
	if (quests.size() < 2) { return ""; }
	for (int i = 0; i < quests.size() - 1; ++i) {
		if (quests[i] == m_id) {
			return quests[i + 1];
		}
	}
	return "";
}

Ref<QS::QuestEntry> QS::QuestEntry::_deserialize(const Dictionary &p_json) {
	ERR_FAIL_COND_V_MSG(!p_json.has("steps"), nullptr, "[QuestEntry::deserialize] Failed to find 'steps' key.");
	ERR_FAIL_COND_V_MSG(p_json["steps"].get_type() != Variant::ARRAY, nullptr, "[QuestEntry::deserialize] 'steps' is not an array.");
	Ref entry{ memnew(QuestEntry) };

	deserialize_json(entry, p_json);

	Array steps = p_json["steps"];
	if (steps.is_empty()) {
		QuestSystem::printerr("'steps' of Quest ID [{0}] is empty.", entry->m_id);
		return nullptr;
	}
	for (const auto &step_json : steps) {
		if (step_json.get_type() != Variant::DICTIONARY) {
			QuestSystem::printerr("Quest ID [{0}] must only contain dictionary values in 'steps'!", entry->m_id);
			return nullptr;
		}
		Ref<QuestStepEntry> step_obj = ObjectSerializationRegistry::get_singleton()->deserialize_json(step_json);
		if (step_obj.is_null()) {
			QuestSystem::printerr("Dictionary in 'steps' of Quest ID [{0}] is invalid.", entry->m_id);
			return nullptr;
		}
		step_obj->set_parent_id(entry->m_id);
		entry->m_steps.push_back(step_obj->get_id());
		QuestSystem::get_singleton()->add_queststep(step_obj);
	}
	return entry;
}

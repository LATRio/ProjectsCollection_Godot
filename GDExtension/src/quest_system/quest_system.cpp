#include "quest_system.hpp"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>

#include "entries/quest_entry.hpp"
#include "entries/quest_step_entry.hpp"
#include "entries/questline_entry.hpp"
#include "object_serialization_registry/object_serialization_registry.hpp"

namespace QS {

void QuestSystem::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_database", "filepath"), &QuestSystem::load_database);
	ClassDB::bind_method(D_METHOD("get_entry", "id"), &QuestSystem::get_entry);
	ClassDB::bind_method(D_METHOD("get_questline", "id"), &QuestSystem::get_questline);
	ClassDB::bind_method(D_METHOD("get_quest", "id"), &QuestSystem::get_quest);
	ClassDB::bind_method(D_METHOD("get_queststep", "id"), &QuestSystem::get_queststep);
	ClassDB::bind_method(D_METHOD("get_questline_ids"), &QuestSystem::get_questline_ids);
	ClassDB::bind_method(D_METHOD("get_quest_ids"), &QuestSystem::get_quest_ids);
	ClassDB::bind_method(D_METHOD("get_free_quest_ids"), &QuestSystem::get_free_quest_ids);
	ClassDB::bind_method(D_METHOD("get_queststep_ids"), &QuestSystem::get_queststep_ids);

	ClassDB::bind_static_method("QuestSystem", D_METHOD("status_to_string", "status"), &QuestSystem::status_to_string);
}

QuestSystem::QuestSystem() {
	m_all_entries_by_status.reserve(static_cast<uint32_t>(EntryStatus::MAX_COUNT));
	m_all_entries_by_status.insert(EntryStatus::UNKNOWN, TypedArray<StringName>());
	m_all_entries_by_status.insert(EntryStatus::AVAILABLE, TypedArray<StringName>());
	m_all_entries_by_status.insert(EntryStatus::INPROGRESS, TypedArray<StringName>());
	m_all_entries_by_status.insert(EntryStatus::COMPLETED, TypedArray<StringName>());
	m_all_entries_by_status.insert(EntryStatus::CANCELLED, TypedArray<StringName>());
	m_all_entries_by_status.insert(EntryStatus::FAILED, TypedArray<StringName>());
	m_all_entries_by_status.insert(EntryStatus::LOCKED, TypedArray<StringName>());
	m_all_entries_by_status.insert(EntryStatus::SKIPPED, TypedArray<StringName>());
	m_all_entries_by_status.insert(EntryStatus::ERROR, TypedArray<StringName>());
}

void QuestSystem::load_database(const String &p_filepath) {
	const Ref db_file{ FileAccess::open(p_filepath, FileAccess::READ) };
	if (!db_file.is_valid()) {
		printerr(FileAccess::get_open_error(), "Failed to open database file {0}.", p_filepath);
	}
	JSON json{};
	if (const Error res{ json.parse(db_file->get_as_text()) }; res != OK) {
		printerr(FileAccess::get_open_error(), "Failed to parse a json. Line {0}: {1}.", json.get_error_line(), json.get_error_message());
	}
	if (json.get_data().get_type() != Variant::DICTIONARY) {
		printerr("Failed to parse a json. Root isn't a dictionary");
	}
	const Dictionary dict_json{ json.get_data() };
	if (dict_json.has("questlines")) {
		Array questlines{dict_json["questlines"]};
		for (const auto &questline_json : questlines) {
			if (questline_json) {
				Ref<QuestlineEntry> entry{ ObjectSerializationRegistry::get_singleton()->deserialize_json(questline_json) };
				if (!entry.is_valid()) {
					printerr("Cannot add null entry! JSON", questline_json);
					continue;
				}
				add_questline(entry);
			}
		}
	}
	if (dict_json.has("free_quests")) {
		Array free_quests{dict_json["free_quests"]};
		for (const auto &quest_json : free_quests) {
			if (quest_json) {
				Ref<QuestEntry> entry{ ObjectSerializationRegistry::get_singleton()->deserialize_json(quest_json) };
				if (!entry.is_valid()) {
					printerr("Cannot add null entry! JSON", quest_json);
					continue;
				}
				add_quest(entry, true);
			}
		}
	}

	for (const auto& entry_id : m_questlines) {
		get_questline(entry_id)->evaluate_availability();
	}

	for (const auto& entry_id : m_free_quests) {
		get_quest(entry_id)->evaluate_availability();
	}
}

void QuestSystem::refresh_entry(const StringName &p_id) {
}

bool QuestSystem::entry_exists(const StringName &p_id) const {
	return m_entries.has(p_id);
}

bool QuestSystem::add_entry(const Ref<Entry> &p_entry) {
	if (entry_exists(p_entry->get_id())) {
		printerr("Entry ID [{0}] already exists!", p_entry->get_id());
		return false;
	}
	m_entries.insert(p_entry->get_id(), p_entry);
	m_all_entries_by_status[p_entry->get_status()].push_back(p_entry);
	return true;
}

void QuestSystem::add_questline(const Ref<QuestlineEntry> &p_questline) {
	if (add_entry(p_questline)) {
		m_questlines.push_back(p_questline->get_id());
	}
}

void QuestSystem::add_quest(const Ref<QuestEntry> &p_quest, const bool is_free) {
	if (add_entry(p_quest)) {
		if (is_free) {
			m_quests.push_back(p_quest->get_id());
		}
		else {
			m_free_quests.push_back(p_quest->get_id());
		}
	}
}

void QuestSystem::add_queststep(const Ref<QuestStepEntry> &p_queststep) {
	if (add_entry(p_queststep)) {
		m_queststeps.push_back(p_queststep->get_id());
	}
}

Ref<Entry> QuestSystem::get_entry(const StringName &p_id) {
	if (entry_exists(p_id)) {
		return m_entries[p_id];
	}
	return nullptr;
}

Ref<QuestlineEntry> QuestSystem::get_questline(const StringName &p_id) {
	if (entry_exists(p_id) && is_questline_id(p_id)) {
		return m_entries[p_id];
	}
	return nullptr;
}

Ref<QuestEntry> QuestSystem::get_quest(const StringName &p_id) {
	if (entry_exists(p_id) && is_quest_id(p_id)) {
		return m_entries[p_id];
	}
	return nullptr;
}

Ref<QuestStepEntry> QuestSystem::get_queststep(const StringName &p_id) {
	if (entry_exists(p_id) && is_queststep_id(p_id)) {
		return m_entries[p_id];
	}
	return nullptr;
}

TypedArray<StringName> QuestSystem::get_questline_ids() const {
	return m_questlines;
}

TypedArray<StringName> QuestSystem::get_quest_ids() const {
	return m_quests;
}

TypedArray<StringName> QuestSystem::get_free_quest_ids() const {
	return m_free_quests;
}

TypedArray<StringName> QuestSystem::get_queststep_ids() const {
	return m_queststeps;
}

TypedArray<StringName> QuestSystem::get_entry_ids_by_status(const EntryStatus p_status) const {
	return m_all_entries_by_status[p_status];
}

TypedArray<StringName> QuestSystem::get_incomplete_entry_ids() const {
	TypedArray<StringName> merged_arr{};
	merged_arr.append_array(m_all_entries_by_status[EntryStatus::UNKNOWN]);
	merged_arr.append_array(m_all_entries_by_status[EntryStatus::AVAILABLE]);
	merged_arr.append_array(m_all_entries_by_status[EntryStatus::INPROGRESS]);
	return merged_arr;
}

bool QuestSystem::is_questline_id(const StringName &p_id) const {
	if (!m_questlines.has(p_id)) {
		printerr("Entry ID [{0}] isn't a questline!", p_id);
		return false;
	}
	return true;
}

bool QuestSystem::is_quest_id(const StringName &p_id) const {
	if (!m_quests.has(p_id)) {
		printerr("Entry ID [{0}] isn't a quest!", p_id);
		return false;
	}
	return true;
}

bool QuestSystem::is_queststep_id(const StringName &p_id) const {
	if (!m_queststeps.has(p_id)) {
		printerr("Entry ID [{0}] isn't a quest step!", p_id);
		return false;
	}
	return true;
}

void QuestSystem::set_entry_status(const StringName &p_id, const EntryStatus p_status) {
	if (!entry_exists(p_id)) {
		printerr("Cannot set a status of a non existent Entry ID [{0}]!", p_id);
		return;
	}
	m_entries[p_id]->set_status(p_status);
}

EntryStatus QuestSystem::get_entry_status(const StringName &p_id) const {
	if (!entry_exists(p_id)) {
		printerr("Cannot get a status of a non existent Entry ID [{0}]!", p_id);
		return EntryStatus::ERROR;
	}
	return m_entries[p_id]->get_status();
}

void QuestSystem::sort_entry_by_status(const StringName &p_id, const EntryStatus p_old_status, EntryStatus p_new_status) {
	if (!m_all_entries_by_status[p_old_status].has(p_id)) {
		printerr("Entry ID [{0}] doesn't exist in a list sorted by status!", p_id);
		return;
	}
	m_all_entries_by_status[p_old_status].erase(p_id);
	m_all_entries_by_status[p_new_status].push_back(p_id);
}

} //namespace QS

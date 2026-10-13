#include "quest_system.hpp"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>

#include "entries/quest_entry.hpp"
#include "entries/quest_step_entry.hpp"
#include "entries/questline_entry.hpp"
#include "object_serialization_registry/object_serialization_registry.hpp"

#define BIND_ENUM_CLASS_CONSTANT(p_enumname, p_constant) \
	ClassDB::bind_integer_constant(get_class_static(), #p_enumname, #p_constant, static_cast<int64_t>(p_enumname::p_constant));

namespace QS {
void QuestSystem::_bind_methods() {
	ClassDB::bind_method(D_METHOD("load_database", "filepath"), &QuestSystem::load_database);
	ClassDB::bind_method(D_METHOD("clear_database"), &QuestSystem::clear_database);
	ClassDB::bind_method(D_METHOD("refresh_entry", "id"), &QuestSystem::refresh_entry);
	ClassDB::bind_method(D_METHOD("get_entry", "id"), &QuestSystem::get_entry);
	ClassDB::bind_method(D_METHOD("get_questline", "id"), &QuestSystem::get_questline);
	ClassDB::bind_method(D_METHOD("get_quest", "id"), &QuestSystem::get_quest);
	ClassDB::bind_method(D_METHOD("get_queststep", "id"), &QuestSystem::get_queststep);
	ClassDB::bind_method(D_METHOD("get_questline_ids"), &QuestSystem::get_questline_ids);
	ClassDB::bind_method(D_METHOD("get_quest_ids"), &QuestSystem::get_quest_ids);
	ClassDB::bind_method(D_METHOD("get_free_quest_ids"), &QuestSystem::get_free_quest_ids);
	ClassDB::bind_method(D_METHOD("get_queststep_ids"), &QuestSystem::get_queststep_ids);
	ClassDB::bind_method(D_METHOD("set_entry_status", "id", "status"), &QuestSystem::set_entry_status);
	ClassDB::bind_method(D_METHOD("get_entry_status"), &QuestSystem::get_entry_status);

	ClassDB::bind_static_method("QuestSystem", D_METHOD("status_to_string", "status"), &QuestSystem::status_to_string);

	BIND_ENUM_CONSTANT(ENTRY_STATUS_UNKNOWN);
	BIND_ENUM_CONSTANT(ENTRY_STATUS_AVAILABLE);
	BIND_ENUM_CONSTANT(ENTRY_STATUS_INPROGRESS);
	BIND_ENUM_CONSTANT(ENTRY_STATUS_COMPLETED);
	BIND_ENUM_CONSTANT(ENTRY_STATUS_CANCELLED);
	BIND_ENUM_CONSTANT(ENTRY_STATUS_FAILED);
	BIND_ENUM_CONSTANT(ENTRY_STATUS_LOCKED);
	BIND_ENUM_CONSTANT(ENTRY_STATUS_SKIPPED);
	BIND_ENUM_CONSTANT(ENTRY_STATUS_ERROR);
	BIND_ENUM_CONSTANT(ENTRY_STATUS_MAX_COUNT);
}

QuestSystem::QuestSystem() {
	m_all_entries_by_status.reserve(static_cast<uint32_t>(ENTRY_STATUS_MAX_COUNT));
	m_all_entries_by_status.insert(ENTRY_STATUS_UNKNOWN, TypedArray<StringName>());
	m_all_entries_by_status.insert(ENTRY_STATUS_AVAILABLE, TypedArray<StringName>());
	m_all_entries_by_status.insert(ENTRY_STATUS_INPROGRESS, TypedArray<StringName>());
	m_all_entries_by_status.insert(ENTRY_STATUS_COMPLETED, TypedArray<StringName>());
	m_all_entries_by_status.insert(ENTRY_STATUS_CANCELLED, TypedArray<StringName>());
	m_all_entries_by_status.insert(ENTRY_STATUS_FAILED, TypedArray<StringName>());
	m_all_entries_by_status.insert(ENTRY_STATUS_LOCKED, TypedArray<StringName>());
	m_all_entries_by_status.insert(ENTRY_STATUS_SKIPPED, TypedArray<StringName>());
	m_all_entries_by_status.insert(ENTRY_STATUS_ERROR, TypedArray<StringName>());
}

void QuestSystem::load_database(const String &p_filepath) {
	const Ref db_file{ FileAccess::open(p_filepath, FileAccess::READ) };
	if (db_file.is_null()) {
		printerr(FileAccess::get_open_error(), "Failed to open database file {0}.", p_filepath);
	}
	Ref json = memnew(JSON);
	if (const Error res = json->parse(db_file->get_as_text()); res != OK) {
		printerr(FileAccess::get_open_error(), "Failed to parse a json. Line {0}: {1}.", json->get_error_line(), json->get_error_message());
	}
	db_file->close();
	if (json->get_data().get_type() != Variant::DICTIONARY) {
		printerr("Failed to parse a json. Root isn't a dictionary");
	}
	const Dictionary dict_json = json->get_data();
	if (dict_json.has("questlines")) {
		Array questlines = dict_json["questlines"];
		for (const auto &questline_json : questlines) {
			if (questline_json.get_type() == Variant::DICTIONARY) {
				Ref<QuestlineEntry> entry = ObjectSerializationRegistry::get_singleton()->deserialize_json(questline_json);
				if (entry.is_null()) {
					printerr("Cannot add null entry! JSON: {0}", questline_json);
					continue;
				}
				add_questline(entry);
			}
		}
	}
	if (dict_json.has("free_quests")) {
		Array free_quests = dict_json["free_quests"];
		for (const auto &quest_json : free_quests) {
			if (quest_json.get_type() == Variant::DICTIONARY) {
				Ref<QuestEntry> entry = ObjectSerializationRegistry::get_singleton()->deserialize_json(quest_json);
				if (entry.is_null()) {
					printerr("Cannot add null entry! JSON: {0}", quest_json);
					continue;
				}
				add_quest(entry, true);
			}
		}
	}

	for (const auto &entry_id : m_questlines) {
		get_questline(entry_id)->evaluate_availability();
	}

	for (const auto &entry_id : m_free_quests) {
		get_quest(entry_id)->evaluate_availability();
	}
}

void QuestSystem::clear_database() {
	m_entries.clear();
	m_questlines.clear();
	m_quests.clear();
	m_free_quests.clear();
	m_queststeps.clear();
	m_all_entries_by_status.clear();
}

void QuestSystem::refresh_entry(const StringName &p_id) {
	get_entry(p_id)->refresh_entry();
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
	m_all_entries_by_status[p_entry->get_status()].push_back(p_entry->get_id());
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
			m_free_quests.push_back(p_quest->get_id());
		} else {
			m_quests.push_back(p_quest->get_id());
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
	merged_arr.append_array(m_all_entries_by_status[ENTRY_STATUS_UNKNOWN]);
	merged_arr.append_array(m_all_entries_by_status[ENTRY_STATUS_AVAILABLE]);
	merged_arr.append_array(m_all_entries_by_status[ENTRY_STATUS_INPROGRESS]);
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
	if (!m_quests.has(p_id) && !m_free_quests.has(p_id)) {
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

void QuestSystem::_set_entry_status(const StringName &p_id, uint32_t p_status) {
	set_entry_status(p_id, static_cast<EntryStatus>(p_status));
}

EntryStatus QuestSystem::get_entry_status(const StringName &p_id) const {
	if (!entry_exists(p_id)) {
		printerr("Cannot get a status of a non existent Entry ID [{0}]!", p_id);
		return ENTRY_STATUS_ERROR;
	}
	return m_entries[p_id]->get_status();
}

uint32_t QuestSystem::_get_entry_status(const StringName &p_id) const {
	return static_cast<uint32_t>(get_entry_status(p_id));
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

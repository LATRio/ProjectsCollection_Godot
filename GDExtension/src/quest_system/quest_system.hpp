#pragma once
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/core/memory.hpp>

#include "common/singleton.hpp"

using namespace godot;

enum EntryStatus {
	ENTRY_STATUS_UNKNOWN = 0,
	ENTRY_STATUS_AVAILABLE,
	ENTRY_STATUS_INPROGRESS,
	ENTRY_STATUS_COMPLETED,
	ENTRY_STATUS_CANCELLED,
	ENTRY_STATUS_FAILED,
	ENTRY_STATUS_LOCKED,
	ENTRY_STATUS_SKIPPED,
	ENTRY_STATUS_ERROR,
	ENTRY_STATUS_MAX_COUNT,
};

namespace QS {

class Entry;
class QuestlineEntry;
class QuestEntry;
class QuestStepEntry;

class QuestSystem final : public Object {
	GDCLASS(QuestSystem, Object);

public:
	static String status_to_string(const EntryStatus p_status) {
		switch (p_status) {
			case ENTRY_STATUS_UNKNOWN:
				return "UNKNOWN";
			case ENTRY_STATUS_AVAILABLE:
				return "AVAILABLE";
			case ENTRY_STATUS_INPROGRESS:
				return "INPROGRESS";
			case ENTRY_STATUS_COMPLETED:
				return "COMPLETED";
			case ENTRY_STATUS_CANCELLED:
				return "CANCELLED";
			case ENTRY_STATUS_FAILED:
				return "FAILED";
			case ENTRY_STATUS_LOCKED:
				return "LOCKED";
			case ENTRY_STATUS_SKIPPED:
				return "SKIPPED";
			case ENTRY_STATUS_ERROR:
				return "ERROR";
			default:
				return "-1";
		}
	}

private:
	QuestSystem();

	HashMap<StringName, Ref<Entry>> m_entries;
	TypedArray<StringName> m_questlines;
	TypedArray<StringName> m_quests;
	TypedArray<StringName> m_free_quests;
	TypedArray<StringName> m_queststeps;
	HashMap<EntryStatus, TypedArray<StringName>> m_all_entries_by_status;

protected:
	static void _bind_methods();

public:
	void load_database(const String &p_filepath);
	void clear_database();
	void refresh_entry(const StringName &p_id);

	[[nodiscard]] bool entry_exists(const StringName &p_id) const;

	bool add_entry(const Ref<Entry> &p_entry);
	void add_questline(const Ref<QuestlineEntry> &p_questline);
	void add_quest(const Ref<QuestEntry> &p_quest, bool is_free = false);
	void add_queststep(const Ref<QuestStepEntry> &p_queststep);

	Ref<Entry> get_entry(const StringName& p_id);
	Ref<QuestlineEntry> get_questline(const StringName& p_id);
	Ref<QuestEntry> get_quest(const StringName& p_id);
	Ref<QuestStepEntry> get_queststep(const StringName& p_id);

	[[nodiscard]] TypedArray<StringName> get_questline_ids() const;
	[[nodiscard]] TypedArray<StringName> get_quest_ids() const;
	[[nodiscard]] TypedArray<StringName> get_free_quest_ids() const;
	[[nodiscard]] TypedArray<StringName> get_queststep_ids() const;

	[[nodiscard]] TypedArray<StringName> get_entry_ids_by_status(EntryStatus p_status) const;
	[[nodiscard]] TypedArray<StringName> get_incomplete_entry_ids() const;

	[[nodiscard]] bool is_questline_id(const StringName& p_id) const;
	[[nodiscard]] bool is_quest_id(const StringName& p_id) const;
	[[nodiscard]] bool is_queststep_id(const StringName& p_id) const;

	void set_entry_status(const StringName& p_id, EntryStatus p_status);
	void _set_entry_status(const StringName& p_id, uint32_t p_status);
	[[nodiscard]] EntryStatus get_entry_status(const StringName& p_id) const;
	[[nodiscard]] uint32_t _get_entry_status(const StringName& p_id) const;

	void sort_entry_by_status(const StringName& p_id, EntryStatus p_old_status, EntryStatus p_new_status);

	template <typename... Args>
	static void printerr(const String &p_arg, const Args &...p_args);
	template <typename... Args>
	static void printerr(Error p_error, const String &p_arg, const Args &...p_args);

	SINGLETON(QuestSystem);
};

template <typename... Args>
void QuestSystem::printerr(const String &p_arg, const Args &...p_args) {
	UtilityFunctions::push_error("[QuestSystem] ", p_arg.format(Array::make(p_args...)));
}

template <typename... Args>
void QuestSystem::printerr(const Error p_error, const String &p_arg, const Args &...p_args) {
	UtilityFunctions::push_error("[QuestSystem][", UtilityFunctions::error_string(p_error), "] ", p_arg.format(Array::make(p_args...)));
}

} //namespace QS

VARIANT_ENUM_CAST(EntryStatus);

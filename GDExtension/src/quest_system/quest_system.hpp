#pragma once
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/core/memory.hpp>

#include "common/singleton.hpp"

using namespace godot;

enum class EntryStatus : uint32_t {
	UNKNOWN = 0,
	AVAILABLE,
	INPROGRESS,
	COMPLETED,
	CANCELLED,
	FAILED,
	LOCKED,
	SKIPPED,
	ERROR,
	MAX_COUNT,
};

VARIANT_ENUM_CAST(EntryStatus);

struct HashMapHasherEntryStatus {
	static _FORCE_INLINE_ uint32_t hash(const EntryStatus p_enum) {
		using UnderlyingType = std::underlying_type_t<EntryStatus>;
		return hash_fmix32(static_cast<UnderlyingType>(p_enum));
	}
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
			case EntryStatus::UNKNOWN:
				return "UNKNOWN";
			case EntryStatus::AVAILABLE:
				return "AVAILABLE";
			case EntryStatus::INPROGRESS:
				return "INPROGRESS";
			case EntryStatus::COMPLETED:
				return "COMPLETED";
			case EntryStatus::CANCELLED:
				return "CANCELLED";
			case EntryStatus::FAILED:
				return "FAILED";
			case EntryStatus::LOCKED:
				return "LOCKED";
			case EntryStatus::SKIPPED:
				return "SKIPPED";
			case EntryStatus::ERROR:
				return "ERROR";
			default:
				return "-1";
		}
	}

private:
	HashMap<StringName, Ref<Entry>> m_entries;
	TypedArray<StringName> m_questlines;
	TypedArray<StringName> m_quests;
	TypedArray<StringName> m_free_quests;
	TypedArray<StringName> m_queststeps;
	HashMap<EntryStatus, TypedArray<StringName>, HashMapHasherEntryStatus> m_all_entries_by_status;

protected:
	static void _bind_methods();

public:
	QuestSystem();

	void load_database(const String &p_filepath);
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
	[[nodiscard]] EntryStatus get_entry_status(const StringName& p_id) const;

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

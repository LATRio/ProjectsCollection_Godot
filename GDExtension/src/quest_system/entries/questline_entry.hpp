#pragma once
#include "entry.hpp"

namespace QS {

class QuestlineEntry final : public Entry {
	GDCLASS(QuestlineEntry, Entry);

	TypedArray<StringName> m_quests;

protected:
	static void _bind_methods();

	bool _set_status_impl(EntryStatus p_new_status) override;
	TypedArray<StringName> _get_child_entry_ids_impl() const override { return m_quests; }

public:
	static String _get_type() {
		return "questline";
	}

	static Ref<QuestlineEntry> _deserialize(const Dictionary &p_json);

};

}

#pragma once
#include "entry.hpp"

namespace QS {

class QuestlineEntry final : public Entry {
	GDCLASS(QuestlineEntry, Entry);

	TypedArray<StringName> m_quests;

protected:
	static void _bind_methods();

public:
	static String _get_type() {
		return "questline";
	}

	StringName get_previous_sibling_entry_id() const override { return ""; }

	StringName get_next_sibling_entry_id() const override { return ""; }

	TypedArray<StringName> get_child_entry_ids() const override { return TypedArray<StringName>(); }

	static Ref<QuestlineEntry> _deserialize(const Dictionary &p_json);
};

}

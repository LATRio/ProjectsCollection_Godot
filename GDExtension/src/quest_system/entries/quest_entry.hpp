#pragma once
#include "entry.hpp"

namespace QS {

class QuestEntry final : public Entry {
	GDCLASS(QuestEntry, Entry);

	TypedArray<StringName> m_steps;

protected:
	static void _bind_methods();

public:
	static String _get_type() {
		return "quest";
	}

	StringName get_previous_sibling_entry_id() const override { return ""; }

	StringName get_next_sibling_entry_id() const override { return ""; }

	TypedArray<StringName> get_child_entry_ids() const override { return TypedArray<StringName>(); }

	static Ref<QuestEntry> _deserialize(const Dictionary &p_json);
};

}

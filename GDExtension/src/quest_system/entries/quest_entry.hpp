#pragma once
#include "entry.hpp"

namespace QS {

class QuestEntry final : public Entry {
	GDCLASS(QuestEntry, Entry);

	TypedArray<StringName> m_steps;

protected:
	static void _bind_methods();

	bool _set_status_impl(EntryStatus p_new_status) override;
	StringName _get_previous_sibling_entry_id_impl() const override;
	StringName _get_next_sibling_entry_id_impl() const override;
	TypedArray<StringName> _get_child_entry_ids_impl() const override { return m_steps; }

public:
	static String _get_type() {
		return "quest";
	}

	static Ref<QuestEntry> _deserialize(const Dictionary &p_json);

};

}

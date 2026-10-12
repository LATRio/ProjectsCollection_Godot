#pragma once
#include "entry.hpp"

namespace QS {

class Objective;

class QuestStepEntry final : public Entry {
	GDCLASS(QuestStepEntry, Entry);

	Ref<Objective> m_objective;

protected:
	static void _bind_methods();

	bool _set_status_impl(EntryStatus p_new_status) override;
	StringName _get_previous_sibling_entry_id_impl() const override;
	StringName _get_next_sibling_entry_id_impl() const override;

public:
	static String _get_type() {
		return "step";
	}

	static Ref<QuestStepEntry> _deserialize(const Dictionary &p_json);
};

}

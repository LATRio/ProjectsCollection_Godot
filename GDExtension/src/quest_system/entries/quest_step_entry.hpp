#pragma once
#include "entry.hpp"

namespace QS {

class Objective;

class QuestStepEntry final : public Entry {
	GDCLASS(QuestStepEntry, Entry);

	Ref<Objective> m_objective;

protected:
	static void _bind_methods();

public:
	static String _get_type() {
		return "questline";
	}

	StringName get_previous_sibling_entry_id() const override { return ""; }

	StringName get_next_sibling_entry_id() const override { return ""; }

	static Ref<QuestStepEntry> _deserialize(const Dictionary &p_json);
};

}

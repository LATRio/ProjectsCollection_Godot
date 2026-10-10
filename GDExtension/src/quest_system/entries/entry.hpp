#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

#include "actions/action.hpp"
#include "quest_system.hpp"

using namespace godot;

namespace QS {

class Action;
class Condition;

class Entry : public Resource {
	GDCLASS(Entry, Resource);

	StringName m_parent_id;
	EntryStatus m_status{};
	Ref<Condition> m_availability_condition;
	Ref<Condition> m_activation_condition;
	Ref<Condition> m_completion_condition;
	Ref<Condition> m_failure_condition;
	TypedArray<Action> m_on_available;
	TypedArray<Action> m_on_active;
	TypedArray<Action> m_on_complete;
	TypedArray<Action> m_on_fail;

protected:
	StringName m_id{};

	static void _bind_methods();

public:
	[[nodiscard]] StringName get_id() const;

	bool set_status(EntryStatus p_new_status);
	[[nodiscard]] EntryStatus get_status() const {
		return m_status;
	}

	void set_parent_id(const StringName &p_parent_id) {
		m_parent_id = p_parent_id;
	}
	StringName get_parent_id() const {
		return m_parent_id;
	}

	virtual StringName get_previous_sibling_entry_id() const;
	virtual StringName get_next_sibling_entry_id() const;
	virtual TypedArray<StringName> get_child_entry_ids() const;

	bool evaluate_availability();
	bool evaluate_activation();
	bool evaluate_completion();
	bool evaluate_failure();

	void refresh_entry();
	void refresh_child_entries();

	static void _deserialize(const Ref<Entry> &p_entry, const Dictionary &p_json);

	GDVIRTUAL0RC(StringName, get_previous_sibling_entry_id);
	GDVIRTUAL0RC(StringName, get_next_sibling_entry_id);
	GDVIRTUAL0RC(TypedArray<StringName>, get_child_entry_ids);
};

} //namespace QS

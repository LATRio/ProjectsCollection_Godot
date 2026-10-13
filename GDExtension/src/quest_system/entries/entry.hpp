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

	EntryStatus m_status{};
	Ref<Condition> m_availability_condition{};
	Ref<Condition> m_activation_condition{};
	Ref<Condition> m_completion_condition{};
	Ref<Condition> m_failure_condition{};
	TypedArray<Action> m_on_available{};
	TypedArray<Action> m_on_active{};
	TypedArray<Action> m_on_complete{};
	TypedArray<Action> m_on_fail{};

	bool base_set_status(EntryStatus p_new_status); // Base Entry's set_status implementation

protected:
	StringName m_id{};
	StringName m_parent_id{};

	static void _bind_methods();

	// Native overridable functions.
	// Overriden by C++. Must have a default definition: cannot register pure C++ interface classes.
	virtual bool _set_status_impl(EntryStatus p_new_status) { return true; }
	virtual StringName _get_previous_sibling_entry_id_impl() const;
	virtual StringName _get_next_sibling_entry_id_impl() const;
	virtual TypedArray<StringName> _get_child_entry_ids_impl() const;
	static void deserialize_json(const Ref<Entry> &p_entry, const Dictionary &p_json);

public:
	void set_id(const StringName &p_id) { m_id = p_id; }

	[[nodiscard]] StringName get_id() const {
		return m_id;
	}

	[[nodiscard]] EntryStatus get_status() const {
		return m_status;
	}

	void set_parent_id(const StringName &p_parent_id) {
		m_parent_id = p_parent_id;
	}

	StringName get_parent_id() const {
		return m_parent_id;
	}

	// Front facing functions.
	// Version that must be called from everywhere else.
	// Calls base_set_status, then if it returns true, call overriden version.
	// TODO: Maybe add an ability to bypass ClassDB call.
	bool set_status(EntryStatus p_new_status);
	StringName get_previous_sibling_entry_id() const;
	StringName get_next_sibling_entry_id() const;
	TypedArray<StringName> get_child_entry_ids() const;

	bool evaluate_availability();
	bool evaluate_activation();
	bool evaluate_completion();
	bool evaluate_failure();

	void refresh_entry();
	void refresh_child_entries() const;

	// Generate boilerplate functions for ClassDB's virtual method slots
	GDVIRTUAL1R(bool, _set_status, uint32_t);
	GDVIRTUAL0RC(StringName, _get_previous_sibling_entry_id);
	GDVIRTUAL0RC(StringName, _get_next_sibling_entry_id);
	GDVIRTUAL0RC(TypedArray<StringName>, _get_child_entry_ids);
};
} //namespace QS

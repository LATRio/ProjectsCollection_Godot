#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

#include "quest_system.hpp"

using namespace godot;

namespace QS {

class Objective : public Resource {
	GDCLASS(Objective, Resource)

	StringName m_parent_step_id;
	bool m_is_completed{};
	bool m_is_failed{};

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_parent_step_id", "parent_step_id"), &Objective::set_parent_step_id);
		ClassDB::bind_method(D_METHOD("get_parent_step_id"), &Objective::get_parent_step_id);
		ClassDB::bind_method(D_METHOD("set_is_completed", "completed"), &Objective::set_is_completed);
		ClassDB::bind_method(D_METHOD("get_is_completed"), &Objective::get_is_completed);
		ClassDB::bind_method(D_METHOD("set_is_failed", "failed"), &Objective::set_is_failed);
		ClassDB::bind_method(D_METHOD("get_is_failed"), &Objective::get_is_failed);

		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "parent_step_id"), "set_parent_step_id", "get_parent_step_id");
		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "is_completed"), "set_is_completed", "get_is_completed");
		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "is_failed"), "set_is_failed", "get_is_failed");

		ClassDB::bind_method(D_METHOD("_on_objective_completed"), &Objective::_on_objective_completed);
		ClassDB::bind_method(D_METHOD("_on_objective_failed"), &Objective::_on_objective_failed);

		GDVIRTUAL_BIND(register_tracker);
		GDVIRTUAL_BIND(unregister_tracker);
		GDVIRTUAL_BIND(_on_objective_updated, "args");
	}

public:
	void set_parent_step_id(const StringName &p_parent_step_id) {
		m_parent_step_id = p_parent_step_id;
	}

	[[nodiscard]] StringName get_parent_step_id() const {
		return m_parent_step_id;
	}

	void set_is_completed(const bool p_completed) {
		m_is_completed = p_completed;
	}

	[[nodiscard]] bool get_is_completed() const {
		return m_is_completed;
	}

	void set_is_failed(const bool p_failed) {
		m_is_failed = p_failed;
	}

	[[nodiscard]] bool get_is_failed() const {
		return m_is_failed;
	}

	GDVIRTUAL0(register_tracker)
	GDVIRTUAL0(unregister_tracker)
	GDVIRTUAL1(_on_objective_updated, Variant)

	void _on_objective_completed() {
		ERR_FAIL_COND_MSG(m_is_failed, String("Current objective of the QuestStep ID [{0}] is already failed!").format(Array::make(m_parent_step_id)));
		m_is_completed = true;
		QuestSystem::get_singleton()->refresh_entry(m_parent_step_id);
	}

	void _on_objective_failed() {
		ERR_FAIL_COND_MSG(m_is_completed, String("Current objective of the QuestStep ID [{0}] is already completed!").format(Array::make(m_parent_step_id)));
		m_is_failed = true;
		QuestSystem::get_singleton()->refresh_entry(m_parent_step_id);
	}
};

} //namespace QS

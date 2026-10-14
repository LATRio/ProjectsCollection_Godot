#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

#include "quest_system.hpp"

using namespace godot;

namespace QS {
class Condition : public Resource {
	GDCLASS(Condition, Resource)

protected:
	StringName m_caller_id;

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_caller_id", "called_id"), &Condition::set_caller_id);
		ClassDB::bind_method(D_METHOD("get_caller_id"), &Condition::get_caller_id);
		ClassDB::bind_method(D_METHOD("evaluate"), &Condition::evaluate);
		ClassDB::bind_method(D_METHOD("notify_parent"), &Condition::notify_parent);

		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "caller_id"), "set_caller_id", "get_caller_id");

		GDVIRTUAL_BIND(_evaluate)
	}

	virtual bool _evaluate_impl() { return false; }

	void notify_parent() const {
		QuestSystem::get_singleton()->refresh_entry(m_caller_id);
	}

public:
	void set_caller_id(const StringName &p_caller_id) {
		m_caller_id = p_caller_id;
	}

	[[nodiscard]] StringName get_caller_id() const {
		return m_caller_id;
	}

	bool evaluate() {
		bool ret{};
		if (GDVIRTUAL_CALL(_evaluate, ret)) {
			return ret;
		}
		return _evaluate_impl();
	}

	GDVIRTUAL0RC(bool, _evaluate)
};
} //namespace QS

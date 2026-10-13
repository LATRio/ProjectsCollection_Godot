#pragma once
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

namespace QS {

class Action : public Resource {
	GDCLASS(Action, Resource)

	StringName m_caller_id;

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_caller_id", "called_id"), &Action::set_caller_id);
		ClassDB::bind_method(D_METHOD("get_caller_id"), &Action::get_caller_id);
		ClassDB::bind_method(D_METHOD("execute"), &Action::execute);

		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "caller_id"), "set_caller_id", "get_caller_id");

		GDVIRTUAL_BIND(_execute);
	}

	virtual void _execute_impl() {}

public:
	void set_caller_id(const StringName &p_caller_id) {
		m_caller_id = p_caller_id;
	}

	[[nodiscard]] StringName get_caller_id() const {
		return m_caller_id;
	}

	void execute() {
		if (!GDVIRTUAL_CALL(_execute)) {
			_execute_impl();
		}
	}

	GDVIRTUAL0(_execute)
};

} //namespace QS

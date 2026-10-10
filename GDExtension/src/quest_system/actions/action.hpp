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

		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "caller_id"), "set_caller_id", "get_caller_id");

		GDVIRTUAL_BIND(execute);
	}

public:
	void set_caller_id(const StringName &p_caller_id) {
		m_caller_id = p_caller_id;
	}

	[[nodiscard]] StringName get_caller_id() const {
		return m_caller_id;
	}

	virtual void execute() {
		GDVIRTUAL_CALL(execute);
	}

	GDVIRTUAL0(execute)
};

} //namespace QS

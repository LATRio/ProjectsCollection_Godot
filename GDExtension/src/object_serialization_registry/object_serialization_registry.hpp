#pragma once
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "common/singleton.hpp"

using namespace godot;

namespace godot {
class Script;
class Resource;
}

class ObjectSerializationRegistry final : public Object {
	GDCLASS(ObjectSerializationRegistry, Object)

	HashMap<String, StringName> m_registry{};
	HashMap<String, Ref<Script>> m_script_registry{};

	void register_class(const StringName& p_classname);
	void register_script(const Ref<Script>& p_script);
	bool registry_has(const String &p_name) const;

protected:
	static void _bind_methods();

public:
	void register_classname(const StringName& p_classname);
	Ref<Resource> deserialize_json(const Dictionary &p_dict_json);

	Ref<Resource> get_class_res(const StringName& p_classname);

	SINGLETON(ObjectSerializationRegistry);
};

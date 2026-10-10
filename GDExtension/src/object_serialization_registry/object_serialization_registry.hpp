#pragma once
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/templates/hash_map.hpp>

#include "common/singleton.hpp"

using namespace godot;

namespace godot {
class Resource;
}

class ObjectSerializationRegistry final : public Object {
	GDCLASS(ObjectSerializationRegistry, Object)

	HashMap<String, StringName> m_registry{};

protected:
	static void _bind_methods();

public:
	void register_class(const StringName& p_classname);
	Ref<Resource> deserialize_json(const Dictionary &p_dict_json);

	SINGLETON(ObjectSerializationRegistry);
};

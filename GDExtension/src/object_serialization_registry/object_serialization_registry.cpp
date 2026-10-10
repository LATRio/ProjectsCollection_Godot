#include "object_serialization_registry.hpp"
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

void ObjectSerializationRegistry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("register_class", "classname"), &ObjectSerializationRegistry::register_class);
	ClassDB::bind_method(D_METHOD("deserialize_json", "dict_json"), &ObjectSerializationRegistry::deserialize_json);
}

void ObjectSerializationRegistry::register_class(const StringName &p_classname) {
	const auto type{ ClassDB::class_call_static(p_classname, "get_type") };
	ERR_FAIL_COND_MSG(type.get_type() == Variant::NIL, "[ObjectSerializationRegistry] Cannot find static method 'get_type'.");
	if (type.get_type() == Variant::NIL) {
		return;
	}
	const MethodBind *deserialize_method{ ClassDB::get_method(p_classname, "deserialize_json") };
	ERR_FAIL_NULL_MSG(deserialize_method, "[ObjectSerializationRegistry] Cannot find static method 'deserialize_json'.");
	ERR_FAIL_COND_MSG(!deserialize_method->has_return(), "[ObjectSerializationRegistry] Cannot find static method 'deserialize_json' with return value.");
	ERR_FAIL_COND_MSG(deserialize_method->get_argument_count() != 1, "[ObjectSerializationRegistry] Cannot find static method 'deserialize_json' with a single argument.");
	const auto& info{deserialize_method->get_arguments_info_list()[0]};
	ERR_FAIL_COND_MSG(info.type != Variant::DICTIONARY, "[ObjectSerializationRegistry] Cannot find static method 'deserialize_json' with 'Dictionary' argument.");

	m_registry.insert(type, p_classname);
}

Ref<Resource> ObjectSerializationRegistry::deserialize_json(const Dictionary &p_dict_json) {
	ERR_FAIL_COND_V_MSG(!p_dict_json.has("type"), nullptr, String("[ObjectSerializationRegistry] Cannot find object key 'type' in dictionary.").format(p_dict_json));
	ERR_FAIL_COND_V_MSG(!m_registry.has(p_dict_json["type"]), nullptr, String("[ObjectSerializationRegistry] '{0}' type isn't registered.").format(p_dict_json["type"]));
	const auto& classname{m_registry[p_dict_json["type"]]};
	return ClassDB::class_call_static(classname, "deserialize_json");
}
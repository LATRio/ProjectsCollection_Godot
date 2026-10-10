#include "object_serialization_registry.hpp"
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

void ObjectSerializationRegistry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("register_class", "classname"), &ObjectSerializationRegistry::register_class);
	ClassDB::bind_method(D_METHOD("deserialize_json", "dict_json"), &ObjectSerializationRegistry::deserialize_json);
}

void ObjectSerializationRegistry::register_class(const StringName &p_classname) {
	const auto type{ ClassDB::class_call_static(p_classname, "_get_type") };
	ERR_FAIL_COND_MSG(!type, "[ObjectSerializationRegistry] Cannot find a static method '_get_type'. " + p_classname);
	//ERR_FAIL_COND_MSG(type.get_type() == Variant::NIL, "[ObjectSerializationRegistry] Cannot find static method '_get_type'. " + p_classname);
	const MethodBind *deserialize_method{ ClassDB::get_method(p_classname, "_deserialize") };
	ERR_FAIL_NULL_MSG(deserialize_method, "[ObjectSerializationRegistry] Cannot find a static method '_deserialize'. " + p_classname);
	ERR_FAIL_COND_MSG(!deserialize_method->has_return(), "[ObjectSerializationRegistry] Cannot find a return value in static method '_deserialize'. " + p_classname);
	ERR_FAIL_COND_MSG(deserialize_method->get_argument_count() != 1, "[ObjectSerializationRegistry] Cannot find a static method '_deserialize' with a single argument. " + p_classname);
	const auto& info{deserialize_method->get_arguments_info_list()[0]};
	//ERR_FAIL_COND_MSG(info.type != Variant::DICTIONARY, "[ObjectSerializationRegistry] Cannot find 'Dictionary' argument in static method '_deserialize'. " + p_classname);

	m_registry.insert(type, p_classname);
}

Ref<Resource> ObjectSerializationRegistry::deserialize_json(const Dictionary &p_dict_json) {
	ERR_FAIL_COND_V_MSG(p_dict_json.is_empty(), nullptr, "[ObjectSerializationRegistry] Passed dict is empty!");
	ERR_FAIL_COND_V_MSG(!p_dict_json.has("type"), nullptr, String("[ObjectSerializationRegistry] Cannot find object key 'type' in dictionary. JSON: {0}").format(Array::make(p_dict_json)));
	ERR_FAIL_COND_V_MSG(!m_registry.has(p_dict_json["type"]), nullptr, String("[ObjectSerializationRegistry] '{0}' type isn't registered. JSON: {0}").format(Array::make(p_dict_json["type"])));
	const auto& classname{m_registry[p_dict_json["type"]]};
	return ClassDB::class_call_static(classname, "_deserialize");
}

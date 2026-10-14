#include "object_serialization_registry.hpp"

#include "godot_cpp/classes/project_settings.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/script.hpp"

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource.hpp>

bool ObjectSerializationRegistry::registry_has(const String &p_name) const {
	return m_registry.has(p_name) || m_script_registry.has(p_name);
}

void ObjectSerializationRegistry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("register_classname", "classname"), &ObjectSerializationRegistry::register_classname);
	ClassDB::bind_method(D_METHOD("deserialize_json", "dict_json"), &ObjectSerializationRegistry::deserialize_json);
}

void ObjectSerializationRegistry::register_classname(const StringName &p_classname) {
	// Is it native class?
	if (ClassDB::class_exists(p_classname)) {
		register_class(p_classname);
		return;
	}
	// Is it GDScript/C# class?
	auto gclasses = ProjectSettings::get_singleton()->get_global_class_list();
	ERR_FAIL_COND_MSG(gclasses.size() == 0, "[ObjectSerializationRegistry] There are no global classes.");
	for (const auto &gclass : gclasses) {
		Dictionary class_dict = gclass;
		if (class_dict["class"] == p_classname) {
			Ref<Script> script = ResourceLoader::get_singleton()->load(class_dict["path"]);
			ERR_FAIL_COND_MSG(script.is_null(), String("[ObjectSerializationRegistry] Couldn't load '{0}' script.").format(Array::make(class_dict["path"])));
			register_script(script);
			return;
		}
	}
}

void ObjectSerializationRegistry::register_class(const StringName &p_classname) {
	bool static_method_found{};
	auto methods = ClassDB::class_get_method_list(p_classname);
	for (const auto &method : methods) {
		Dictionary method_dict = method;
		if (static_cast<String>(method_dict["name"]) == "_get_type") {
			if (static_cast<int64_t>(method_dict["flags"]) & METHOD_FLAG_STATIC) {
				static_method_found = true;
				break;
			}
		}
	}
	ERR_FAIL_COND_MSG(!static_method_found, "[ObjectSerializationRegistry] Cannot find a static method '_get_type'. " + p_classname);
	const auto type{ ClassDB::class_call_static(p_classname, "_get_type") };

	UtilityFunctions::print("[ObjectSerializationRegistry] Registering. Type: " + static_cast<StringName>(type) + ", Class: " + p_classname);

	ERR_FAIL_COND_MSG(m_registry.has(type), String("[ObjectSerializationRegistry] Registry already has type {0} registered. {1}").format(Array::make(type, p_classname)));
	ERR_FAIL_COND_MSG(!ClassDB::class_has_method(p_classname, "_deserialize"), "[ObjectSerializationRegistry] Cannot find a static method '_deserialize'. " + p_classname);
	const MethodBind *deserialize_method{ ClassDB::get_method(p_classname, "_deserialize") };
	ERR_FAIL_COND_MSG(!deserialize_method->has_return(), "[ObjectSerializationRegistry] Cannot find a return value in static method '_deserialize'. " + p_classname);
	ERR_FAIL_COND_MSG(deserialize_method->get_argument_count() != 1, "[ObjectSerializationRegistry] Cannot find a static method '_deserialize' with a single argument. " + p_classname);

	m_registry.insert(type, p_classname);
}

void ObjectSerializationRegistry::register_script(const Ref<Script> &p_script) {
	bool static_method__get_type_found{ p_script->has_script_method("_get_type") };
	bool static_method__deserialize_found{ p_script->has_script_method("_deserialize") };
	// auto methods = p_script->get_method_list();
	// for (const auto &method : methods) {
	// 	Dictionary method_dict = method;
	// 	if (!static_method__get_type_found && static_cast<String>(method_dict["name"]) == "_get_type") {
	// 		UtilityFunctions::printerr("Found _get_type");
	// 		if (static_cast<int64_t>(method_dict["flags"]) & METHOD_FLAG_STATIC) {
	// 			static_method__get_type_found = true;
	// 			break;
	// 		}
	// 	}
	// 	if (!static_method__deserialize_found && static_cast<String>(method_dict["name"]) == "_deserialize") {
	// 		UtilityFunctions::printerr("Found _deserialize");
	// 		if (static_cast<int64_t>(method_dict["flags"]) & METHOD_FLAG_STATIC) {
	// 			static_method__deserialize_found = true;
	// 			break;
	// 		}
	// 	}
	// }
	ERR_FAIL_COND_MSG(!static_method__get_type_found, "[ObjectSerializationRegistry] Cannot find a static method '_get_type'. " + p_script->get_global_name());
	ERR_FAIL_COND_MSG(!static_method__deserialize_found, "[ObjectSerializationRegistry] Cannot find a static method '_deserialize'. " + p_script->get_global_name());
	const auto type = p_script->call("_get_type");

	UtilityFunctions::print("[ObjectSerializationRegistry] Registering. Type: " + static_cast<StringName>(type) + ", Class: " + p_script->get_global_name());

	ERR_FAIL_COND_MSG(m_script_registry.has(type), String("[ObjectSerializationRegistry] Registry already has type {0} registered. {1}").format(Array::make(type, p_script->get_global_name())));

	m_script_registry.insert(type, p_script);
}

Ref<Resource> ObjectSerializationRegistry::deserialize_json(const Dictionary &p_dict_json) {
	ERR_FAIL_COND_V_MSG(p_dict_json.is_empty(), nullptr, "[ObjectSerializationRegistry] Passed dict is empty!");
	ERR_FAIL_COND_V_MSG(!p_dict_json.has("type"), nullptr, String("[ObjectSerializationRegistry] Cannot find object key 'type' in dictionary. JSON: {0}").format(Array::make(p_dict_json)));
	ERR_FAIL_COND_V_MSG(!registry_has(p_dict_json["type"]), nullptr, String("[ObjectSerializationRegistry] '{0}' type isn't registered. JSON: {0}").format(Array::make(p_dict_json)));
	if (m_registry.has(p_dict_json["type"])) {
		return ClassDB::class_call_static(m_registry[p_dict_json["type"]], "_deserialize", p_dict_json);
	}
	if (m_script_registry.has(p_dict_json["type"])) {
		return m_script_registry[p_dict_json["type"]]->call("_deserialize", p_dict_json);
	}
	return nullptr;
}

Ref<Resource> ObjectSerializationRegistry::get_class_res(const StringName &p_classname) {
	if (m_script_registry.has(p_classname)) {
		return m_script_registry[p_classname]->call("new");
	}
	if (m_registry.has(p_classname)) {
		return ClassDB::instantiate(m_registry[p_classname]);
	}
	UtilityFunctions::printerr(String("[ObjectSerializationRegistry] Failed to find '{0}' class.").format(Array::make(p_classname)));
	return nullptr;
}

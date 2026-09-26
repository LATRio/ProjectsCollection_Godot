extends Node

var _registry: Dictionary[String, Script]
# Also possible with inheritance?


func _ready() -> void:
	# TODO: Have a scripts that registers all custom serializers. Game finds files
	# in specific folder(s) that match a pattern "register_serializers_{UniqueAuthorName?}.gd".
	# Game then calls a static function "register()" that registers all the
	# custom serializer script classes.
	
	# Entries
	register_class(QuestlineEntry)
	register_class(QuestEntry)
	register_class(QuestStepEntry)
	
	# Actions
	register_class(SetEntryStatus_Action)
	register_class(ActivateNextSiblingEntry_Action)
	
	# Conditions
	register_class(Comparison_Condition)
	register_class(Logical_Condition)
	register_class(True_Condition)
	register_class(False_Condition)
	register_class(PreviousSiblingEntryIsCompleted_Condition)
	
	# Objectives
	register_class(DamagePlayer_Objective)
	register_class(HealPlayer_Objective)


func register_class(script: Script) -> void:
	var found_type_prop_in_script := false
	for prop in script.get_property_list():
		if prop.has("type"):
			found_type_prop_in_script = true
			break
	if not found_type_prop_in_script:
		push_error("Cannot register a class without 'type' property! Class: {0}".format([script.get_global_name()]))
		return
	
	var found_deserialize_method := false
	for method in script.get_script_method_list():
		if method["name"] == "deserialize":
			if method["args"].size() == 1:
				if method["args"][0]["type"] == Variant.Type.TYPE_DICTIONARY:
					found_deserialize_method = true
					break
	
	if not found_deserialize_method:
		push_error("Cannot register class that doesn't have a function 'deserialize' function! Class: {0}".format([script.get_global_name()]))
		return
	
	# All requirements are met. Proceed.
	_registry[script.type] = script


func deserialize_json(dict_json: Dictionary) -> RefCounted:
	if not dict_json.has("type"):
		push_error("JSON Dictionary doesn't contain 'type' key. JSON: ", dict_json)
		return null
	if not _registry.has(dict_json["type"]):
		push_error("Class with 'type' {0} isn't registered!".format([dict_json["type"]]))
		return null
	return _registry[dict_json["type"]].deserialize(dict_json)
	

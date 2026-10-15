#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/engine.hpp>

#include "actions/action.hpp"
#include "conditions/condition.hpp"
#include "entries/quest_entry.hpp"
#include "entries/quest_step_entry.hpp"
#include "entries/questline_entry.hpp"
#include "object_serialization_registry/object_serialization_registry.hpp"
#include "objectives/objective.hpp"
#include "quest_system.hpp"
#include "conditions/allchildentriescompleted_condition.hpp"
#include "conditions/anychildentryfailed_condition.hpp"
#include "conditions/logical_condition.hpp"
#include "conditions/objectivecompleted_condition.hpp"
#include "conditions/objectivefailed_condition.hpp"
#include "conditions/true_condition.hpp"
#include "conditions/false_condition.hpp"
#include "conditions/previoussiblingentrycompleted_condition.hpp"

using namespace godot;

static void initialize_gdextension_types(const ModuleInitializationLevel p_level)
{
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(QS::QuestSystem);
	GDREGISTER_CLASS(ObjectSerializationRegistry);
	QS::QuestSystem::create_singleton();
	ObjectSerializationRegistry::create_singleton();
	Engine::get_singleton()->register_singleton("QuestSystem", QS::QuestSystem::get_singleton());
	Engine::get_singleton()->register_singleton("ObjectSerializationRegistry", ObjectSerializationRegistry::get_singleton());
	GDREGISTER_CLASS(QS::Condition);
	GDREGISTER_CLASS(QS::Action);
	GDREGISTER_CLASS(QS::Objective);
	GDREGISTER_ABSTRACT_CLASS(QS::Entry);
	GDREGISTER_CLASS(QS::QuestStepEntry);
	GDREGISTER_CLASS(QS::QuestEntry);
	GDREGISTER_CLASS(QS::QuestlineEntry);
	GDREGISTER_CLASS(QS::AllChildEntriesCompleted_Condition);
	GDREGISTER_CLASS(QS::AnyChildEntryFailed_Condition);
	GDREGISTER_CLASS(QS::ObjectiveCompleted_Condition);
	GDREGISTER_CLASS(QS::ObjectiveFailedCondition);
	GDREGISTER_CLASS(QS::Logical_Condition);
	GDREGISTER_CLASS(QS::True_Condition);
	GDREGISTER_CLASS(QS::False_Condition);
	GDREGISTER_CLASS(QS::PreviousSiblingEntryCompleted_Condition)
	{
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::QuestlineEntry::get_class_static());
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::QuestEntry::get_class_static());
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::QuestStepEntry::get_class_static());
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::AllChildEntriesCompleted_Condition::get_class_static());
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::AnyChildEntryFailed_Condition::get_class_static());
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::ObjectiveCompleted_Condition::get_class_static());
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::ObjectiveFailedCondition::get_class_static());
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::Logical_Condition::get_class_static());
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::True_Condition::get_class_static());
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::False_Condition::get_class_static());
		ObjectSerializationRegistry::get_singleton()->register_classname(QS::PreviousSiblingEntryCompleted_Condition::get_class_static());
	}
}

static void uninitialize_gdextension_types(const ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	Engine::get_singleton()->unregister_singleton("ObjectSerializationRegistry");
	Engine::get_singleton()->unregister_singleton("QuestSystem");
	ObjectSerializationRegistry::destroy_singleton();
	QS::QuestSystem::destroy_singleton();
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT project_collection_lib_init(const GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		const GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		init_obj.register_initializer(initialize_gdextension_types);
		init_obj.register_terminator(uninitialize_gdextension_types);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}

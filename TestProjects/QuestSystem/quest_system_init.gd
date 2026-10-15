extends Node


func _ready() -> void:
	ObjectSerializationRegistry.register_classname("ActivateNextSiblingEntry_Action")
	ObjectSerializationRegistry.register_classname("SetEntryStatus_Action")
	
	ObjectSerializationRegistry.register_classname("PlayerDamaged_Condition")
	
	ObjectSerializationRegistry.register_classname("Comparison_Condition")
	
	ObjectSerializationRegistry.register_classname("DamagePlayer_Objective")
	ObjectSerializationRegistry.register_classname("HealPlayer_Objective")

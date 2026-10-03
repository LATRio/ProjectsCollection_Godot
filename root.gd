extends Control
# TODO: export array of PackedScenes and generate buttons
# TODO: Make it possible to return to this screen


func _on_composition_demo_pressed() -> void:
	get_tree().change_scene_to_file("res://TestProjects/Composition/Scenes/CompositionDemo.tscn")
	# Compare gdscript vs gdextension performance. explore multiple scenarios?
	# Compare node vs renderingserver performance. test incrementally each feature?
	# maybe physics? node vs server?


func _on_exit_to_desktop_pressed() -> void:
	get_tree().quit(0)

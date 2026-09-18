extends Control
# TODO: export array of PackedScenes and generate buttons
# TODO: Make it possible to return to this screen

func _on_button_pressed() -> void:
	get_tree().change_scene_to_file("res://TestProjects/Composition/Scenes/CompositionDemo.tscn")

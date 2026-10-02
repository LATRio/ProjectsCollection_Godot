@tool
extends EditorPlugin

const MAIN_SCREEN = preload("uid://c5fi6k8d8536p")

var main_screen_instance: Control = null


func _enable_plugin() -> void:
	# Add autoloads here.
	pass


func _disable_plugin() -> void:
	# Remove autoloads here.
	pass


func _enter_tree() -> void:
	main_screen_instance = MAIN_SCREEN.instantiate()
	EditorInterface.get_editor_main_screen().add_child(main_screen_instance)
	_make_visible(false)
	pass


func _exit_tree() -> void:
	# Clean-up of the plugin goes here.
	pass


func _has_main_screen() -> bool:
	return true


func _make_visible(visible: bool) -> void:
	if main_screen_instance:
		if visible:
			main_screen_instance.show()
		else:
			main_screen_instance.hide()


func _get_plugin_name() -> String:
	return "Quest"


func _get_plugin_icon() -> Texture2D:
	return null

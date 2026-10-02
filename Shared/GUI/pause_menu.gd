extends CanvasLayer

var is_mouse_withing_window := true


func _unhandled_key_input(event: InputEvent) -> void:
	if not is_mouse_withing_window:
		return
	if event.is_action_pressed("escape"):
		if visible:
			_hide()
		else:
			_show()


func _notification(what: int) -> void:
	match what:
		NOTIFICATION_WM_MOUSE_ENTER:
			is_mouse_withing_window = true
		NOTIFICATION_WM_MOUSE_EXIT:
			is_mouse_withing_window = false


func _hide() -> void:
	hide()
	get_tree().paused = false
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func _show() -> void:
	show()
	get_tree().paused = true
	Input.mouse_mode = Input.MOUSE_MODE_VISIBLE


func _on_resume_pressed() -> void:
	_hide()


func _on_return_to_main_menu_pressed() -> void:
	get_tree().paused = true
	get_tree().change_scene_to_file("res://Root.tscn")

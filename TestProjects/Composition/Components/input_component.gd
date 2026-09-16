class_name InputComponent
extends Node

# TODO: Replace with movement component? or PlayerController and use it to
# "possess" suitable nodes

var move_dir := Vector2.ZERO
var jump_pressed := false
var heal_pressed := false
var hurt_pressed := false

var mouse_out_of_window := false

func _ready() -> void:
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func _unhandled_input(event: InputEvent) -> void:
	if mouse_out_of_window:
		return
	if event is InputEventKey:
		if event.keycode == KEY_ALT:
			if event.pressed:
				Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
			else:
				Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
	elif event is InputEventMouseButton:
		if event.pressed:
			Input.mouse_mode = Input.MOUSE_MODE_CAPTURED


func _notification(what: int) -> void:
	match what:
		NOTIFICATION_WM_MOUSE_EXIT:
			mouse_out_of_window = true
		NOTIFICATION_WM_MOUSE_ENTER:
			mouse_out_of_window = false


func update() -> void:
	move_dir = Vector2.ZERO
	if Input.mouse_mode != Input.MOUSE_MODE_CAPTURED:
		return
	move_dir = Input.get_vector("left", "right", "up", "down")
	jump_pressed = Input.is_action_just_pressed("jump")
	heal_pressed = Input.is_action_just_pressed("heal")
	hurt_pressed = Input.is_action_just_pressed("hurt")

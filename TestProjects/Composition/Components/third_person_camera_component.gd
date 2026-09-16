class_name ThirdPersonCameraComponent
extends Node3D

@onready var spring_arm: SpringArm3D = $SpringArm3D

@export var mouse_sensitivity := 0.002
@export var smoothness := 15.0 # Higher - faster/snappier, Lower - smoother/slower

@export var camera_zoom_speed := 0.05
@export var min_camera_distance := 0.7
@export var max_camera_distance := 4.0
# Raise camera's zoom target higher to focus on the face instead of the chest
@export var camera_transition_range_min := 0.7
@export var camera_transition_range_max := 1.5
@export var camera_adjusted_height := 0.28

var target_rotation := Vector2.ZERO
var camera_forward := Vector3.ZERO
var camera_right := Vector3.ZERO

func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventMouseMotion:
		if Input.mouse_mode == Input.MOUSE_MODE_CAPTURED:
			# up - positive, right - positive
			target_rotation.x -= event.relative.x * mouse_sensitivity
			target_rotation.y -= event.relative.y * mouse_sensitivity
			
			target_rotation.y = clampf(target_rotation.y, deg_to_rad(-90.0), deg_to_rad(90.0))
	if event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_WHEEL_UP:
			spring_arm.spring_length = clampf(spring_arm.spring_length - camera_zoom_speed, min_camera_distance, max_camera_distance)
		elif event.button_index == MOUSE_BUTTON_WHEEL_DOWN:
			spring_arm.spring_length = clampf(spring_arm.spring_length + camera_zoom_speed, min_camera_distance, max_camera_distance)


func _adjust_camera_height() -> void:
	var percentage = (spring_arm.spring_length - camera_transition_range_min) / (camera_transition_range_max - camera_transition_range_min)
	percentage = 1.0 - clampf(percentage, 0.0, 1.0)
	spring_arm.position.y = camera_adjusted_height * percentage


func tick(delta: float) -> void:
	_adjust_camera_height()
	# Interpolate instead of snapping. Makes camera movement feel more smooth and polished.
	spring_arm.rotation.x = lerp_angle(spring_arm.rotation.x, target_rotation.y, smoothness * delta)
	spring_arm.rotation.y = lerp_angle(spring_arm.rotation.y, target_rotation.x, smoothness * delta)
	camera_forward = spring_arm.transform.basis.z # +Z is forward
	camera_right = spring_arm.transform.basis.x # +Z is
	camera_forward.y = 0.0
	camera_right.y = 0.0

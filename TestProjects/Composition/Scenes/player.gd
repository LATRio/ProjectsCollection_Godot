class_name Player
extends CharacterBody3D

@onready var input_component: InputComponent = %InputComponent
@onready var movement_component: MovementComponent = %MovementComponent
@onready var health_component: HealthComponent = %HealthComponent
@onready var third_person_camera_component: ThirdPersonCameraComponent = $ThirdPersonCameraComponent


func _ready() -> void:
	health_component.died.connect(_player_died)


func _physics_process(delta: float) -> void:
	# PROCESS INPUTS
	input_component.update()
	
	# PROCESS CAMERA INPUT
	third_person_camera_component.tick(delta)
		
	# PROCESS MOVEMENT
	movement_component.move_dir = input_component.move_dir
	movement_component.camera_forward = third_person_camera_component.camera_forward
	movement_component.camera_right = third_person_camera_component.camera_right
	movement_component.wants_jump = input_component.jump_pressed
	movement_component.tick(delta)
	
	# PROCESS HEALTH
	if input_component.heal_pressed:
		health_component.heal(5.0)
	if input_component.hurt_pressed:
		health_component.damage(10.0)


func _player_died() -> void:
	get_tree().reload_current_scene()

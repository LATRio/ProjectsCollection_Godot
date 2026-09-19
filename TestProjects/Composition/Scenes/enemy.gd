class_name Enemy
extends CharacterBody3D

@onready var movement_component: MovementComponent = %MovementComponent
@onready var health_component: HealthComponent = %HealthComponent
@onready var navigation_agent_3d: NavigationAgent3D = %NavigationAgent3D


func _physics_process(delta: float) -> void:
	if not navigation_agent_3d.is_navigation_finished():
		movement_component.move_towards_position(navigation_agent_3d.get_next_path_position())
	movement_component.camera_forward = transform.basis.z
	movement_component.camera_right = transform.basis.x
	movement_component.camera_forward.y = 0.0
	movement_component.camera_right.y = 0.0
	movement_component.tick(delta)


func _on_navigation_agent_3d_path_changed() -> void:
	movement_component.needs_to_move = true
	navigation_agent_3d.debug_enabled = true


func _on_navigation_agent_3d_navigation_finished() -> void:
	print("navigation finished")
	movement_component.needs_to_move = false
	movement_component.move_dir = Vector2.ZERO
	navigation_agent_3d.debug_enabled = false

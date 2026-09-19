class_name MovementComponent
extends Node
# Can be put on anything capable of moving.
# Note: Extract model's logic into specialized component if necessary.
# Controlling a model through an AnimationTree would be better.

@export var body: CharacterBody3D
@export var model: Node3D
@export var speed := 4.0
@export var jump_velocity := 12.0
@export var gravity_multiplier := 3.0

var move_dir := Vector2.ZERO
var camera_forward := Vector3.ZERO
var camera_right := Vector3.ZERO
var wants_jump := false

var needs_to_move := false

func tick(delta: float) -> void:
	if not body:
		return
	
	# Top Down Movement
	# Apply left and right movement to the Camera's right vector
	# Apply up and down movement to the Camera's up vector
	var direction := Vector3(camera_right * move_dir.x + camera_forward * move_dir.y).normalized()
	body.velocity.x = direction.x * speed
	body.velocity.z = direction.z * speed
	
	# Gravity
	if not body.is_on_floor():
		body.velocity += body.get_gravity() * gravity_multiplier * delta
	
	# Jump
	if wants_jump:
		body.velocity.y = jump_velocity
		wants_jump = false
	
	body.move_and_slide()
	
	# Face model towards movement direction. This is fine for primitive models.
	if model and direction.length_squared() > 0.001:
		var look_dir := -Vector3(direction.x, 0.0, direction.z).normalized()
		model.look_at(model.global_position + look_dir, Vector3.UP)


# Utility movement function. Useful for cutscenes or NPCs
func move_towards_position(target_pos: Vector3) -> void:
	var target_dir := target_pos - body.global_position
	move_dir = Vector2(target_dir.x, target_dir.z)


# Utility model function. Useful if model needs to lock onto something.
func look_toward_position(_target_pos: Vector3) -> void:
	# Placeholder. Maybe extract to the specialized component
	pass

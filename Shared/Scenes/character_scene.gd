class_name CharacterScene
extends Node3D

@onready var parent: CharacterBody3D = get_parent()
@onready var anim_tree: AnimationTree = $AnimationTree
@onready var right_hand_slot: BoneAttachment3D = %RightHandSlot

@export_node_path("Node3D") var root_motion_track: NodePath

@export var blend_speed := 10.0

var move_dir := Vector2.ZERO
var camera_forward := Vector3.ZERO
var is_on_floor := true
var actual_value := 0.0


func _ready() -> void:
	#anim_tree.root_motion_track = root_motion_track
	pass


func _process(delta: float) -> void:
	var forward_backward_value := Vector2(parent.velocity.x, parent.velocity.z).normalized().length()
	actual_value = move_toward(actual_value, forward_backward_value, delta * blend_speed)
	anim_tree.set("parameters/Locomotion/Movement/blend_position", actual_value)
	anim_tree.set("parameters/conditions/is_on_floor", parent.is_on_floor())
	anim_tree.set("parameters/conditions/not_on_floor", not parent.is_on_floor())


func equip_weapon() -> void:
	#right_hand_slot
	pass


func attack() -> void:
	anim_tree["parameters/Locomotion/SwordAttackOneShot/request"] = AnimationNodeOneShot.ONE_SHOT_REQUEST_FIRE


func _on_animation_tree_animation_finished(anim_name: StringName) -> void:
	if anim_name == "SwordAttack":
		# SwordAttack OneShot animation finished
		pass

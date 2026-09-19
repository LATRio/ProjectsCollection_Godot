class_name MovingPlatform
extends Node3D

@onready var animation_player: AnimationPlayer = $AnimationPlayer
@onready var path_follow_3d: PathFollow3D = $Path3D/PathFollow3D

var active := false
var moving_forward := true


func _on_interactable_component_interacted(_interactor: InteractorComponent) -> void:
	if active:
		moving_forward = not moving_forward
	else:
		active = true
		moving_forward = is_equal_approx(path_follow_3d.progress_ratio, 0.0)
	if moving_forward:
		animation_player.play(&"move")
	else:
		animation_player.play_backwards(&"move")


func _on_animation_player_animation_finished(_anim_name: StringName) -> void:
	active = false

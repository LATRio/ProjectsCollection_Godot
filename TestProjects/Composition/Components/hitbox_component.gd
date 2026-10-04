class_name HitboxComponent
extends Area3D
# Offensive

@export var damage := 0


func hitscan() -> void:
	for area in get_overlapping_areas():
		if area is HurtboxComponent:
			pass

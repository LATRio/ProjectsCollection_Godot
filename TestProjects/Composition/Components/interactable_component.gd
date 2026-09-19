class_name InteractableComponent
extends Area3D

@onready var collision_shape_3d: CollisionShape3D = %CollisionShape3D

@export var radius := 0.5

signal interacted(interactor: InteractorComponent)


func _ready() -> void:
	(collision_shape_3d.shape as SphereShape3D).radius = radius


func interact(interactor: InteractorComponent) -> void:
	interacted.emit(interactor)

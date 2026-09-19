class_name PickableComponent
extends Area3D
# TODO: Derive from interactable component?

@onready var collision_shape_3d: CollisionShape3D = %CollisionShape3D

@export var radius := 0.5
#@export var pickup_item: Item # TODO: implement Item/Inventory system


func _ready() -> void:
	(collision_shape_3d.shape as SphereShape3D).radius = radius


# Mockup of how Interaction system would work
#func interact(interactor: InteractorComponent) -> void:
#	pass


func pickup() -> void:
	# TODO: Add assigned item to the player's (or whatever/whoever that has
	# InteractorComponent interacts with it) inventory
	get_parent().queue_free()

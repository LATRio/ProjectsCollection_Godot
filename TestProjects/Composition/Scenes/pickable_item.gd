class_name PickableItem
extends RigidBody3D

@onready var interactable_component: InteractableComponent = $InteractableComponent

#@export var item_to_claim: InventoryItem # Mockup for a claimable item


func _on_interactable_component_interacted(_interactor: InteractorComponent) -> void:
	# TODO: Find a way to add an inventory item to the interactor's parent
	queue_free()

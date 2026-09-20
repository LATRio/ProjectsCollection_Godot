class_name InteractorComponent
extends Area3D


func get_overlapping_interactables() -> Array[InteractableComponent]:
	var interactables: Array[InteractableComponent] = []
	for area in get_overlapping_areas():
		if area is InteractableComponent:
			interactables.push_back(area)
	return interactables


func interact_with_first_overlapping_interactable():
	# TODO: Find a way to determine which item must be interacted with.
	# Maybe add priority value to the InteractableComponent?
	var interactables := get_overlapping_interactables()
	if interactables.is_empty():
		return
	get_overlapping_interactables().front().interact(self)

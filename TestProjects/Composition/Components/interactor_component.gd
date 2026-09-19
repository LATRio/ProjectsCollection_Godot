class_name InteractorComponent
extends Area3D


func get_overlapping_interactibles() -> Array[PickableComponent]:
	var all_pickables: Array[PickableComponent] = []
	for area in get_overlapping_areas():
		if area is PickableComponent:
			all_pickables.push_back(area)
	return all_pickables


func get_first_overlapping_interactible() -> PickableComponent:
	return get_overlapping_interactibles().front()

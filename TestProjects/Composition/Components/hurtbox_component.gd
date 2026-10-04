class_name HurtboxComponent
extends Area3D
# Defensive

@export var health_component: HealthComponent


# TODO: Damage must have information on who dealt it. 
func register_damage(amount: float) -> void:
	if health_component:
		health_component.damage(amount)

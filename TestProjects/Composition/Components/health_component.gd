class_name HealthComponent
extends Node

signal health_changed(current: float, max: float)
signal died # Use EventBus instead?

@export var max_health := 100.0
var current_health := max_health


func _ready() -> void:
	_emit()


func damage(amount: float) -> void:
	if amount < 0.0:
		push_error("[HealthComponent] Damage value cannot be negative! Amount: ", amount)
		return
	current_health -= amount
	if current_health <= 0.0:
		current_health = 0.0
		died.emit()
		EventBus.player_died.emit()
	_emit()
	EventBus.player_received_damage.emit(amount)


func heal(amount: float) -> void:
	assert(amount >= 0.0)
	current_health = clamp(current_health + amount, 0.0, max_health)
	_emit()


func _emit() -> void:
	health_changed.emit(current_health, max_health)
	print("HP: %d / %d" % [current_health, max_health])

extends Node3D


func _ready() -> void:
	QuestSystem.load_database("res://TestProjects/Database/quest_db.json")


func _process(_delta: float) -> void:
	pass

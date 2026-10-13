extends Node3D


func _enter_tree() -> void:
	QuestSystem.load_database("res://TestProjects/Database/quest_db.json")


func _process(_delta: float) -> void:
	pass


func _notification(what: int) -> void:
	match what:
		NOTIFICATION_EXIT_TREE:
			QuestSystem.clear_database()
		_:
			pass

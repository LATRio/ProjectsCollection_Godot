extends CanvasLayer

@onready var tree: Tree = %Tree


func _ready() -> void:
	tree.hide_root = true
	tree.set_column_title(0, "ID")
	tree.set_column_title(1, "Status")
	var root := tree.create_item()
	for questline_id in QuestSystem.questlines:
		var questline := QuestSystem.get_questline(questline_id)
		var questline_item := tree.create_item(root)
		questline_item.set_text(0, "Questline: " + str(questline.id))
		questline_item.set_text(1, QuestSystem.status_to_string(questline.get_status()))
		questline.entry_updated.connect(update_entry_item.bind(questline_item, questline.id))
		for quest_id in questline.quests:
			var quest := QuestSystem.get_quest(quest_id)
			var quest_item := tree.create_item(questline_item)
			quest_item.set_text(0, "Quest: " + str(quest.id))
			quest_item.set_text(1, QuestSystem.status_to_string(quest.get_status()))
			quest.entry_updated.connect(update_entry_item.bind(quest_item, quest.id))
			for queststep_id in quest.steps:
				var queststep := QuestSystem.get_queststep(queststep_id)
				var queststep_item := tree.create_item(quest_item)
				queststep_item.set_text(0, "Queststep: " + str(queststep.id))
				queststep_item.set_text(1, QuestSystem.status_to_string(queststep.get_status()))
				queststep.entry_updated.connect(update_entry_item.bind(queststep_item, queststep.id))
	var freequests_item := tree.create_item(root)
	freequests_item.set_text(0, "Free Quests")
	for quest_id in QuestSystem.free_quests:
		var quest := QuestSystem.get_quest(quest_id)
		var quest_item := tree.create_item(freequests_item)
		quest_item.set_text(0, "Quest: " + str(quest.id))
		quest_item.set_text(1, QuestSystem.status_to_string(quest.get_status()))
		quest.entry_updated.connect(update_entry_item.bind(quest_item, quest.id))
		for queststep_id in quest.steps:
			var queststep := QuestSystem.get_queststep(queststep_id)
			var queststep_item := tree.create_item(quest_item)
			queststep_item.set_text(0, "Queststep: " + str(queststep.id))
			queststep_item.set_text(1, QuestSystem.status_to_string(queststep.get_status()))
			queststep.entry_updated.connect(update_entry_item.bind(queststep_item, queststep.id))


func update_entry_item(item: TreeItem, entry_id: int) -> void:
	item.set_text(1, QuestSystem.status_to_string(QuestSystem.get_entry(entry_id).get_status()))

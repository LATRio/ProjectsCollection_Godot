extends Tree


func _ready() -> void:
	hide_root = true
	set_column_title(0, "ID")
	set_column_title(1, "Status")
	var root := create_item()
	
	for questline_id in QuestSystem.get_questline_ids():
		var questline := QuestSystem.get_questline(questline_id)
		var questline_item := create_item(root)
		questline_item.set_text(0, "Questline: " + questline.id)
		questline_item.set_text(1, QuestSystem.status_to_string(questline.get_status()))
		questline.entry_updated.connect(update_entry_item.bind(questline_item, questline.id))
		for quest_id in questline.quests:
			var quest := QuestSystem.get_quest(quest_id)
			var quest_item := create_item(questline_item)
			quest_item.set_text(0, "Quest: " + quest.id)
			quest_item.set_text(1, QuestSystem.status_to_string(quest.get_status()))
			quest.entry_updated.connect(update_entry_item.bind(quest_item, quest.id))
			for queststep_id in quest.steps:
				var queststep := QuestSystem.get_queststep(queststep_id)
				var queststep_item := create_item(quest_item)
				queststep_item.set_text(0, "Queststep: " + queststep.id)
				queststep_item.set_text(1, QuestSystem.status_to_string(queststep.get_status()))
				queststep.entry_updated.connect(update_entry_item.bind(queststep_item, queststep.id))
	var freequests_item := create_item(root)
	freequests_item.set_text(0, "Free Quests")
	for quest_id in QuestSystem.get_free_quest_ids():
		var quest := QuestSystem.get_quest(quest_id)
		var quest_item := create_item(freequests_item)
		quest_item.set_text(0, "Quest: " + quest.id)
		quest_item.set_text(1, QuestSystem.status_to_string(quest.get_status()))
		quest.entry_updated.connect(update_entry_item.bind(quest_item, quest.id))
		for queststep_id in quest.steps:
			var queststep := QuestSystem.get_queststep(queststep_id)
			var queststep_item := create_item(quest_item)
			queststep_item.set_text(0, "Queststep: " + queststep.id)
			queststep_item.set_text(1, QuestSystem.status_to_string(queststep.get_status()))
			queststep.entry_updated.connect(update_entry_item.bind(queststep_item, queststep.id))


func update_entry_item(item: TreeItem, entry_id: StringName) -> void:
	item.set_text(1, QuestSystem.status_to_string(QuestSystem.get_entry(entry_id).get_status()))

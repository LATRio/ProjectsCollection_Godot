# meta-description: Default template
class_name _CLASS_
extends _BASE_


func execute() -> void:
	pass


static func deserialize(json: Dictionary) -> _CLASS_:
	var _CLASS_SNAKE_CASE_ := _CLASS_.new()
	return _CLASS_SNAKE_CASE_

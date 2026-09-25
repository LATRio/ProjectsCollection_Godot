# meta-description: Condition derived template
class_name _CLASS_
extends _BASE_

static var type := "_CLASS_"


func evaluate() -> bool:
	return false


static func deserialize(json: Dictionary) -> _CLASS_:
	var _CLASS_SNAKE_CASE_ := _CLASS_.new()
	return _CLASS_SNAKE_CASE_

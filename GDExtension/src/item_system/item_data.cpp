#include "item_data.hpp"

namespace IS {
void ItemData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_id", "id"), &ItemData::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &ItemData::get_id);
	ClassDB::bind_method(D_METHOD("set_item_name", "item_name"), &ItemData::set_item_name);
	ClassDB::bind_method(D_METHOD("get_item_name"), &ItemData::get_item_name);
	ClassDB::bind_method(D_METHOD("set_item_desc", "item_desc"), &ItemData::set_item_desc);
	ClassDB::bind_method(D_METHOD("get_item_desc"), &ItemData::get_item_desc);
	ClassDB::bind_method(D_METHOD("set_stack_max_size", "stack_max_size"), &ItemData::set_stack_max_size);
	ClassDB::bind_method(D_METHOD("get_stack_max_size"), &ItemData::get_stack_max_size);

	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "id"), "set_id", "get_id");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_name"), "set_item_name", "get_item_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "item_desc"), "set_item_desc", "get_item_desc");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stack_max_size"), "set_stack_max_size", "get_stack_max_size");
}
} // QS
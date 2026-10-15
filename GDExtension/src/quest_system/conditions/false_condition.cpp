#include "false_condition.hpp"

namespace QS {
void False_Condition::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_get_type"), &False_Condition::_get_type);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_deserialize", "json"), &False_Condition::_deserialize);
}

bool False_Condition::_evaluate_impl() {
	return false;
}

Ref<False_Condition> False_Condition::_deserialize(const Dictionary &p_json) {
	return memnew(False_Condition);
}
}

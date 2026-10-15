#include "true_condition.hpp"

namespace QS {
void True_Condition::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_get_type"), &True_Condition::_get_type);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("_deserialize", "json"), &True_Condition::_deserialize);
}

bool True_Condition::_evaluate_impl() {
	return true;
}

Ref<True_Condition> True_Condition::_deserialize(const Dictionary &p_json) {
	return memnew(True_Condition);
}
}

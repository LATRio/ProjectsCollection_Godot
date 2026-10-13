#include "entry.hpp"
#include "conditions/condition.hpp"
#include "object_serialization_registry/object_serialization_registry.hpp"

namespace QS {
void Entry::_bind_methods() {
	ADD_SIGNAL(MethodInfo("entry_updated"));

	// Overridable functions
	ClassDB::bind_method(D_METHOD("set_id", "id"), &Entry::set_id);
	ClassDB::bind_method(D_METHOD("get_id"), &Entry::get_id);
	ClassDB::bind_method(D_METHOD("set_status", "new_status"), &Entry::set_status);
	ClassDB::bind_method(D_METHOD("get_status"), &Entry::get_status);
	ClassDB::bind_method(D_METHOD("get_previous_sibling_entry_id"), &Entry::get_previous_sibling_entry_id);
	ClassDB::bind_method(D_METHOD("get_next_sibling_entry_id"), &Entry::get_next_sibling_entry_id);
	ClassDB::bind_method(D_METHOD("get_child_entry_ids"), &Entry::get_child_entry_ids);

	ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "id"), "set_id", "get_id");

	ClassDB::bind_static_method(get_class_static(), D_METHOD("deserialize_json", "entry", "dict_json"), &Entry::deserialize_json);

	// Virtual method slots in GDScript/C#. For example: when calling actual 'set_status',
	// it will call '_set_status' that was "overriden" by GDScript/C# or '_set_status_impl'
	// that was overriden by derived C++ classes.
	GDVIRTUAL_BIND(_set_status, "new_status");
	GDVIRTUAL_BIND(_get_previous_sibling_entry_id);
	GDVIRTUAL_BIND(_get_next_sibling_entry_id);
	GDVIRTUAL_BIND(_get_child_entry_ids);
}

bool Entry::base_set_status(const EntryStatus p_new_status) {
	// Skip if status is actually unchanged
	ERR_FAIL_COND_V_MSG(m_status == p_new_status, false, String("[QuestSystem] Trying to set the same status '{0}' to Entry ID [{1}]").format(Array::make(p_new_status, get_id())));
	QuestSystem::get_singleton()->sort_entry_by_status(m_id, m_status, p_new_status);
	m_status = p_new_status;
	switch (m_status) {
		case ENTRY_STATUS_AVAILABLE: {
			if (!m_on_available.is_empty()) {
				for (const Ref<Action> action : m_on_available) {
					action->execute();
				}
			}
			refresh_entry();
			break;
		}
		case ENTRY_STATUS_INPROGRESS: {
			if (!m_on_active.is_empty()) {
				for (const Ref<Action> action : m_on_active) {
					action->execute();
				}
			}
			refresh_entry();
			break;
		}
		case ENTRY_STATUS_COMPLETED: {
			if (!m_on_complete.is_empty()) {
				for (const Ref<Action> action : m_on_complete) {
					action->execute();
				}
			}
			if (m_parent_id.is_empty()) {
				QuestSystem::get_singleton()->get_entry(m_parent_id)->refresh_entry();
			}
			break;
		}
		case ENTRY_STATUS_FAILED: {
			if (!m_on_fail.is_empty()) {
				for (const Ref<Action> action : m_on_fail) {
					action->execute();
				}
			}
			if (m_parent_id.is_empty()) {
				QuestSystem::get_singleton()->get_entry(m_parent_id)->refresh_entry();
			}
			break;
		}
		default:
			break;
	}

	emit_signal("entry_update");
	return true;
}

StringName Entry::_get_previous_sibling_entry_id_impl() const {
	QuestSystem::printerr("{0} [{1}] didn't implement 'get_previous_sibling_entry_id'.", get_class_static(), m_id);
	return "";
}

StringName Entry::_get_next_sibling_entry_id_impl() const {
	QuestSystem::printerr("{0} [{1}] didn't implement 'get_next_sibling_entry_id'.", get_class_static(), m_id);
	return "";
}

TypedArray<StringName> Entry::_get_child_entry_ids_impl() const {
	return TypedArray<StringName>();
}

bool Entry::set_status(EntryStatus p_new_status) {
	if (base_set_status(p_new_status)) {
		bool ret{};
		// If overriden in GDScript/C#
		if (!GDVIRTUAL_CALL(_set_status, p_new_status, ret)) {
			return ret;
		}
		// If overriden in C++. Returns true if not overriden.
		return _set_status_impl(p_new_status);
	}
	return false;
}

StringName Entry::get_previous_sibling_entry_id() const {
	if (!m_parent_id.is_empty()) {
		// Not point in continuing if there is no parent
		StringName ret;
		if (GDVIRTUAL_CALL(_get_previous_sibling_entry_id, ret)) {
			return ret;
		}
		return _get_previous_sibling_entry_id_impl();
	}
	return "";
}

StringName Entry::get_next_sibling_entry_id() const {
	if (!m_parent_id.is_empty()) {
		// Not point in continuing if there is no parent
		StringName ret;
		if (GDVIRTUAL_CALL(_get_next_sibling_entry_id, ret)) {
			return ret;
		}
		return _get_next_sibling_entry_id_impl();
	}
	return "";
}

TypedArray<StringName> Entry::get_child_entry_ids() const {
	TypedArray<StringName> ret;
	if (GDVIRTUAL_CALL(_get_child_entry_ids, ret)) {
		return ret;
	}
	return _get_child_entry_ids_impl();
}

bool Entry::evaluate_availability() {
	if (m_availability_condition.is_null() || m_availability_condition->evaluate()) {
		set_status(ENTRY_STATUS_AVAILABLE);
		return true;
	}
	return false;
}

bool Entry::evaluate_activation() {
	if (m_activation_condition.is_null() || m_activation_condition->evaluate()) {
		set_status(ENTRY_STATUS_INPROGRESS);
		return true;
	}
	return false;
}

bool Entry::evaluate_completion() {
	if (m_completion_condition.is_null() || m_completion_condition->evaluate()) {
		set_status(ENTRY_STATUS_COMPLETED);
		return true;
	}
	return false;
}

bool Entry::evaluate_failure() {
	if (m_failure_condition.is_null() || m_failure_condition->evaluate()) {
		set_status(ENTRY_STATUS_FAILED);
		return true;
	}
	return false;
}

void Entry::refresh_entry() {
	switch (m_status) {
		case ENTRY_STATUS_UNKNOWN: {
			evaluate_availability();
			break;
		}
		case ENTRY_STATUS_AVAILABLE: {
			evaluate_activation();
			break;
		}
		case ENTRY_STATUS_INPROGRESS: {
			refresh_child_entries();
			if (!evaluate_completion()) {
				evaluate_failure();
			}
			break;
		}
		// TODO: Mark uncompleted child entries as skipped
		default:
			break;
	}
}

void Entry::refresh_child_entries() const {
	for (const auto &child_id : get_child_entry_ids()) {
		Ref child = QuestSystem::get_singleton()->get_entry(child_id);
		if (child.is_valid()) {
			child->refresh_entry();
		}
	}
}

void Entry::deserialize_json(const Ref<Entry> &p_entry, const Dictionary &p_json) {
	ERR_FAIL_NULL_MSG(p_entry, "[Entry::deserialize_json] 'p_entry' is null.");
	ERR_FAIL_COND_MSG(!p_json.has("id"), "[Entry::deserialize_json] Failed to find 'id' key.");
	p_entry->m_id = p_json["id"];

	if (p_json.has("availability_condition")) {
		p_entry->m_availability_condition = ObjectSerializationRegistry::get_singleton()->deserialize_json(p_json["availability_condition"]);
		if (p_entry->m_availability_condition.is_valid()) {
			p_entry->m_availability_condition->set_caller_id(p_entry->get_id());
		}
	}

	if (p_json.has("activation_condition")) {
		p_entry->m_activation_condition = ObjectSerializationRegistry::get_singleton()->deserialize_json(p_json["activation_condition"]);
		if (p_entry->m_activation_condition.is_valid()) {
			p_entry->m_activation_condition->set_caller_id(p_entry->get_id());
		}
	}

	if (p_json.has("completion_condition")) {
		p_entry->m_completion_condition = ObjectSerializationRegistry::get_singleton()->deserialize_json(p_json["completion_condition"]);
	} else {
		if (static_cast<String>(p_json["type"]) == "step") {
			p_entry->m_completion_condition = ObjectSerializationRegistry::get_singleton()->get_class_res("ObjectiveCompleted");
		} else {
			p_entry->m_completion_condition = ObjectSerializationRegistry::get_singleton()->get_class_res("AllChildEntriesComplete");
		}
	}
	p_entry->m_completion_condition->set_caller_id(p_entry->get_id());

	if (p_json.has("failure_condition")) {
		p_entry->m_failure_condition = ObjectSerializationRegistry::get_singleton()->deserialize_json(p_json["failure_condition"]);
	} else {
		if (static_cast<String>(p_json["type"]) == "step") {
			p_entry->m_failure_condition = ObjectSerializationRegistry::get_singleton()->get_class_res("ObjectiveFailed");
		} else {
			p_entry->m_failure_condition = ObjectSerializationRegistry::get_singleton()->get_class_res("AnyChildEntryFailed");
		}
	}
	p_entry->m_failure_condition->set_caller_id(p_entry->get_id());

	if (p_json.has("on_available")) {
		Array actions = p_json["on_available"];
		for (const auto &action_json : actions) {
			p_entry->m_on_available.push_back(ObjectSerializationRegistry::get_singleton()->deserialize_json(action_json));
			cast_to<Action>(p_entry->m_on_available.back())->set_caller_id(p_entry->get_id());
		}
	}
	if (p_json.has("on_active")) {
		Array actions = p_json["on_active"];
		for (const auto &action_json : actions) {
			p_entry->m_on_available.push_back(ObjectSerializationRegistry::get_singleton()->deserialize_json(action_json));
			cast_to<Action>(p_entry->m_on_active.back())->set_caller_id(p_entry->get_id());
		}
	}
	if (p_json.has("on_complete")) {
		Array actions = p_json["on_complete"];
		for (const auto &action_json : actions) {
			p_entry->m_on_complete.push_back(ObjectSerializationRegistry::get_singleton()->deserialize_json(action_json));
			cast_to<Action>(p_entry->m_on_complete.back())->set_caller_id(p_entry->get_id());
		}
	}
	if (p_json.has("on_fail")) {
		Array actions = p_json["on_fail"];
		for (const auto &action_json : actions) {
			p_entry->m_on_fail.push_back(ObjectSerializationRegistry::get_singleton()->deserialize_json(action_json));
			cast_to<Action>(p_entry->m_on_fail.back())->set_caller_id(p_entry->get_id());
		}
	}
}
} //namespace QS

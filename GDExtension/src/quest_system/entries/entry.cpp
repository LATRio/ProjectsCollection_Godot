#include "entry.hpp"
#include "conditions/condition.hpp"
#include "object_serialization_registry/object_serialization_registry.hpp"

namespace QS {

void Entry::_bind_methods() {
	ADD_SIGNAL(MethodInfo("entry_updated"));
}

StringName Entry::get_id() const {
	return m_id;
}

bool Entry::set_status(const EntryStatus p_new_status) {
	ERR_FAIL_COND_V(m_status == p_new_status, false);
	QuestSystem::get_singleton()->sort_entry_by_status(m_id, m_status, p_new_status);
	m_status = p_new_status;
	switch (m_status) {
		case EntryStatus::AVAILABLE: {
			if (!m_on_available.is_empty()) {
				for (const Ref<Action> action : m_on_available) {
					action->execute();
				}
			}
			refresh_entry();
			break;
		}
		case EntryStatus::INPROGRESS: {
			if (!m_on_active.is_empty()) {
				for (const Ref<Action> action : m_on_active) {
					action->execute();
				}
			}
			refresh_entry();
			break;
		}
		case EntryStatus::COMPLETED: {
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
		case EntryStatus::FAILED: {
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

StringName Entry::get_previous_sibling_entry_id() const {
	StringName ret;
	if (GDVIRTUAL_CALL(get_previous_sibling_entry_id, ret)) {
		return ret;
	}
	return "";
}

StringName Entry::get_next_sibling_entry_id() const {
	StringName ret;
	if (GDVIRTUAL_CALL(get_next_sibling_entry_id, ret)) {
		return ret;
	}
	return "";
}

TypedArray<StringName> Entry::get_child_entry_ids() const {
	TypedArray<StringName> ret;
	if (GDVIRTUAL_CALL(get_child_entry_ids, ret)) {
		return ret;
	}
	return TypedArray<StringName>();
}

bool Entry::evaluate_availability() {
	if (!m_availability_condition.is_valid() || m_availability_condition->evaluate()) {
		set_status(EntryStatus::AVAILABLE);
		return true;
	}
	return false;
}

bool Entry::evaluate_activation() {
	if (!m_activation_condition.is_valid() || m_activation_condition->evaluate()) {
		set_status(EntryStatus::INPROGRESS);
		return true;
	}
	return false;
}

bool Entry::evaluate_completion() {
	if (!m_completion_condition.is_valid() || m_completion_condition->evaluate()) {
		set_status(EntryStatus::COMPLETED);
		return true;
	}
	return false;
}

bool Entry::evaluate_failure() {
	if (!m_failure_condition.is_valid() || m_failure_condition->evaluate()) {
		set_status(EntryStatus::FAILED);
		return true;
	}
	return false;
}

void Entry::refresh_entry() {
	switch (m_status) {
		case EntryStatus::UNKNOWN: {
			evaluate_availability();
			break;
		}
		case EntryStatus::AVAILABLE: {
			evaluate_activation();
			break;
		}
		case EntryStatus::INPROGRESS: {
			evaluate_completion();
			break;
		}
			// TODO: Mark uncompleted child entries as skipped
		default:
			break;
	}
}

void Entry::refresh_child_entries() {
	for (const auto& child_id : get_child_entry_ids()) {
		Ref child{QuestSystem::get_singleton()->get_entry(child_id)};
		if (child.is_valid()) {
			child->refresh_entry();
		}
	}
}

void Entry::_deserialize(const Ref<Entry> &p_entry, const Dictionary &p_json) {
	ERR_FAIL_COND_MSG(!p_json.has("id"), "[Entry::deserialize] Failed to find 'id' key.");
	p_entry->m_id = p_json["id"];

	if (p_json.has("availability_condition")) {
		p_entry->m_availability_condition = ObjectSerializationRegistry::get_singleton()->deserialize_json(p_json["availability_condition"]);
		if (p_entry->m_availability_condition.is_valid()) {
			p_entry->m_availability_condition->set_caller_id(p_entry->m_id);
		}
	}

	if (p_json.has("activation_condition")) {
		p_entry->m_activation_condition = ObjectSerializationRegistry::get_singleton()->deserialize_json(p_json["activation_condition"]);
		if (p_entry->m_activation_condition.is_valid()) {
			p_entry->m_activation_condition->set_caller_id(p_entry->m_id);
		}
	}

	if (p_json.has("completion_condition")) {
		p_entry->m_completion_condition = ObjectSerializationRegistry::get_singleton()->deserialize_json(p_json["completion_condition"]);
	} else {
	}
	p_entry->m_completion_condition->set_caller_id(p_entry->m_id);

	if (p_json.has("failure_condition")) {
		p_entry->m_failure_condition = ObjectSerializationRegistry::get_singleton()->deserialize_json(p_json["failure_condition"]);
	} else {
	}
	p_entry->m_failure_condition->set_caller_id(p_entry->m_id);

	if (p_json.has("on_available")) {
		Array actions = p_json["on_available"];
		for (const auto &action_json : actions) {
			p_entry->m_on_available.push_back(ObjectSerializationRegistry::get_singleton()->deserialize_json(action_json));
			cast_to<Action>(p_entry->m_on_available.back())->set_caller_id(p_entry->m_id);
		}
	}
	if (p_json.has("on_active")) {
		Array actions = p_json["on_active"];
		for (const auto &action_json : actions) {
			p_entry->m_on_available.push_back(ObjectSerializationRegistry::get_singleton()->deserialize_json(action_json));
			cast_to<Action>(p_entry->m_on_active.back())->set_caller_id(p_entry->m_id);
		}
	}
	if (p_json.has("on_complete")) {
		Array actions = p_json["on_complete"];
		for (const auto &action_json : actions) {
			p_entry->m_on_complete.push_back(ObjectSerializationRegistry::get_singleton()->deserialize_json(action_json));
			cast_to<Action>(p_entry->m_on_complete.back())->set_caller_id(p_entry->m_id);
		}
	}
	if (p_json.has("on_fail")) {
		Array actions = p_json["on_fail"];
		for (const auto &action_json : actions) {
			p_entry->m_on_fail.push_back(ObjectSerializationRegistry::get_singleton()->deserialize_json(action_json));
			cast_to<Action>(p_entry->m_on_fail.back())->set_caller_id(p_entry->m_id);
		}
	}
}

} //namespace QS

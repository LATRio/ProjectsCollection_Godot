#pragma once
#include "godot_cpp/classes/resource.hpp"

using namespace godot;

namespace IS {

class ItemData : public Resource {
	GDCLASS(ItemData, Resource)

	StringName m_id;
	String m_item_name;
	String m_item_desc;
	uint32_t m_stack_max_size{};
	// Some visual representation of the item. Maybe another class?
	// TODO: Add sorting feature. Maybe using priority system or something else.
	// Might also make it overridable? depends on how interaction system will be implemented.
	// Or treat that as secondary priority that overrides primary one, but respects it within the same secondary priority.

protected:
	static void _bind_methods();

public:
	void set_id(const StringName &p_id) { m_id = p_id; }
	[[nodiscard]] StringName get_id() const { return m_id; }
	void set_item_name(const String &p_item_name) { m_item_name = p_item_name; }
	[[nodiscard]] String get_item_name() const { return m_item_name; }
	void set_item_desc(const String &p_item_desc) { m_item_desc = p_item_desc; }
	[[nodiscard]] String get_item_desc() const { return m_item_desc; }
	void set_stack_max_size(const uint32_t &p_stack_max_size) { m_stack_max_size = p_stack_max_size; }
	[[nodiscard]] uint32_t get_stack_max_size() const { return m_stack_max_size; }
};

} //namespace IS

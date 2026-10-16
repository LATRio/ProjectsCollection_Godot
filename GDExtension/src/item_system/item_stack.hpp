#pragma once
#include <godot_cpp/classes/ref_counted.hpp>
#include "item_data.hpp"

using namespace godot;

namespace IS {
class ItemData;

class ItemStack : public RefCounted {
	GDCLASS(ItemStack, RefCounted)

	Ref<ItemData> m_item_data;
	uint32_t m_quantity{};

protected:
	static void _bind_methods();

public:
	void set_item_data(const Ref<ItemData> &p_item_data) { m_item_data = p_item_data; }
	[[nodiscard]] Ref<ItemData> get_item_data() const { return m_item_data; }
	void set_quantity(const uint32_t p_quantity) { m_quantity = p_quantity; }
	[[nodiscard]] uint32_t get_quantity() const { return m_quantity; }
	uint32_t add_quantity(uint32_t p_quantity);

	[[nodiscard]] bool is_full() const;
};

} //namespace IS

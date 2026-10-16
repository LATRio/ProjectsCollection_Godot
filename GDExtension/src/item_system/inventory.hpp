#pragma once
#include <godot_cpp/classes/ref_counted.hpp>

using namespace godot;

namespace IS {
class ItemData;
class ItemStack;

class Inventory : public RefCounted {
	GDCLASS(Inventory, RefCounted)

	// TODO: Might consider to replace this with couple virtual functions in case
	// deriving inventory classes need a different way of storing ItemStacks(or some other item holders),
	// but still might reuse already implemented default behaviors.
	TypedArray<Ref<ItemStack>> m_item_stacks{};

protected:
	static void _bind_methods();

public:
	// TODO: Does it even need a return value?
	bool add_item(const Ref<ItemData> &p_item, uint32_t p_quantity);
	bool remove_item(const Ref<ItemData> &p_item, uint32_t p_quantity);
	uint32_t get_total_qty(const Ref<ItemData> &p_item);

	TypedArray<Ref<ItemStack>> get_stacks() const { return m_item_stacks; }

	// TODO: Add an ability to sort item stacks?
};
} //namespace IS

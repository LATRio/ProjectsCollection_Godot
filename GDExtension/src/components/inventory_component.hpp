#pragma once
#include "godot_cpp/classes/node.hpp"

namespace IS {
class Inventory;
class ItemData;
}

using namespace godot;

class InventoryComponent : public Node{
	GDCLASS(InventoryComponent, Node);

	Ref<IS::Inventory> m_inventory;

protected:
	static void _bind_methods();

public:
	InventoryComponent();

	void add_item(const Ref<IS::ItemData> &p_item_data, uint32_t p_quantity) const;
	void remove_item(const Ref<IS::ItemData> &p_item_data, uint32_t p_quantity) const;

	void print_inventory() const;
};

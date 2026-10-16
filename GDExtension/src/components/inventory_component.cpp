#include "inventory_component.hpp"

#include "item_system/inventory.hpp"
#include "item_system/item_stack.hpp"
#include "item_system/item_data.hpp"

void InventoryComponent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("add_item", "item", "qty"), &InventoryComponent::add_item);
	ClassDB::bind_method(D_METHOD("remove_item", "item", "qty"), &InventoryComponent::remove_item);
	ClassDB::bind_method(D_METHOD("print_inventory"), &InventoryComponent::print_inventory);
}

InventoryComponent::InventoryComponent() {
	m_inventory = memnew(IS::Inventory);
}

void InventoryComponent::add_item(const Ref<IS::ItemData> &p_item_data, const uint32_t p_quantity) const {
	if (!m_inventory->add_item(p_item_data, p_quantity)) {
		UtilityFunctions::print("[InventoryComponent] Failed to add an item!");
	}
}

void InventoryComponent::remove_item(const Ref<IS::ItemData> &p_item_data, const uint32_t p_quantity) const {
	if (!m_inventory->remove_item(p_item_data, p_quantity)) {
		UtilityFunctions::print("[InventoryComponent] Failed to remove an item!");
	}
}

void InventoryComponent::print_inventory() const {
	String output;
	for (const auto& stack : m_inventory->get_stacks()) {
		const Ref<IS::ItemStack> item_stack = stack;
		output += String("ItemStack: id[{0}], qty[{1}]\n").format(Array::make(item_stack->get_item_data()->get_id(), item_stack->get_quantity()));
	}
	UtilityFunctions::print(output);
}
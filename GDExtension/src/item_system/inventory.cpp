#include "inventory.hpp"

#include "item_system/item_stack.hpp"

namespace IS {
void Inventory::_bind_methods() {
}

bool Inventory::add_item(const Ref<ItemData> &p_item, const uint32_t p_quantity) {
	ERR_FAIL_COND_V_MSG(p_quantity == 0u, false, "[ItemSystem][Inventory] Trying to add 0 qty of " + p_item->get_id());

	uint32_t remainder{ p_quantity };
	// Fill existing stacks, if they exist and not already full
	for (const auto &stack : m_item_stacks) {
		Ref<ItemStack> item_stack = stack;
		if (item_stack->get_item_data() == p_item && !item_stack->is_full()) {
			remainder = item_stack->add_quantity(remainder);
			if (remainder == 0u) {
				return true;
			}
		}
	}
	// TODO: Maybe inventory has some limitations. Might implement it by calling a virtual function
	// that will get overridden by deriving inventory classes like "WeightedInventory".
	// And if limit is reached and can no longer add new ItemStack, return true.

	// Start adding new stacks
	do {
		Ref new_item_stack = memnew(ItemStack);
		new_item_stack->set_item_data(p_item);
		if (remainder > p_item->get_stack_max_size()) {
			new_item_stack->set_quantity(p_item->get_stack_max_size());
			remainder -= p_item->get_stack_max_size();
		} else {
			new_item_stack->set_quantity(remainder);
			remainder = 0u;
		}
		m_item_stacks.push_back(new_item_stack);
	} while (remainder > 0u);
	return true;
}

bool Inventory::remove_item(const Ref<ItemData> &p_item, const uint32_t p_quantity) {
	ERR_FAIL_COND_V_MSG(p_quantity == 0u, false, "[ItemSystem][Inventory] Trying to remove 0 qty of " + p_item->get_id());
	const uint32_t total_qty{ get_total_qty(p_item) };
	ERR_FAIL_COND_V_MSG(total_qty < p_quantity, false, String("[ItemSystem][Inventory] Inventory only has {0} of {1}, but trying to remove {2}!").format(Array::make(total_qty, p_item->get_id(), p_quantity)));

	TypedArray<Ref<ItemStack>> to_be_removed_stacks;

	uint32_t remainder{ p_quantity };
	for (auto i = m_item_stacks.size() - 1; i >= 0 && remainder != 0u; --i) {
		Ref<ItemStack> item_stack = m_item_stacks[i];
		if (item_stack->get_item_data() == p_item) {
			if (remainder >= item_stack->get_quantity()) {
				remainder -= item_stack->get_quantity();
				m_item_stacks.erase(item_stack);
				// Not need to touch 'i' since we're going in reverse
			} else {
				item_stack->set_quantity(item_stack->get_quantity() - remainder);
				remainder = 0u;
			}
		}
	}
	ERR_FAIL_COND_V_MSG(remainder != 0u, false, String("[ItemSystem][Inventory] Something went wrong. Remainder supposed to be 0 by the end, but {0} is still remaining!").format(Array::make(remainder)));
	return true;
}

uint32_t Inventory::get_total_qty(const Ref<ItemData> &p_item) {
	uint32_t total_qty{};
	for (const auto &stack : m_item_stacks) {
		const Ref<ItemStack> item_stack = stack;
		if (item_stack->get_item_data() == p_item) {
			total_qty += item_stack->get_quantity();
		}
	}
	return total_qty;
}
} //namespace IS
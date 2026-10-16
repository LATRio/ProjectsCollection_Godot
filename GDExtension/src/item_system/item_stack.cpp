#include "item_stack.hpp"

namespace IS {
void ItemStack::_bind_methods() {
}

uint32_t ItemStack::add_quantity(const uint32_t p_quantity) {
	// Anomaly fixing code (either from some bug or cheat). Probably unnecessary.
	if (m_quantity > m_item_data->get_stack_max_size()) {
		const uint32_t remainder{m_quantity - m_item_data->get_stack_max_size()};
		m_quantity = m_item_data->get_stack_max_size();
		ERR_FAIL_V_MSG(remainder, "[ItemSystem][ItemStack] Quantity was higher than allowed stack limit of ItemData " + m_item_data->get_id());
	}

	m_quantity += p_quantity;
	if (m_quantity > m_item_data->get_stack_max_size()) {
		const uint32_t remainder{m_quantity - m_item_data->get_stack_max_size()};
		m_quantity = m_item_data->get_stack_max_size();
		return remainder;
	}
	return 0u;
}

bool ItemStack::is_full() const {
	ERR_FAIL_COND_V_MSG(m_quantity > m_item_data->get_stack_max_size(), true, "[InventorySystem][ItemStack] ItemStack quantity is higher than allowed max stack limit!");
	return m_quantity == m_item_data->get_stack_max_size();
}
} //namespace IS
#include "weapon.h"
#include <engine/impl/offsets.h>
#include <include/includes.h>

auto Weapon::set_skin(int id) -> void
{
	recode::driver.write<int>(weapon + offsets::skin_id, id);
}

auto Weapon::set_model(const std::string& model_name, int model_index) -> void
{

}

auto Weapon::set_glow(int id) -> void
{
	recode::driver.write<uint8_t>(base_address + 0x298, id);
	recode::driver.write<int>(base_address + 0x278, 2);  // m_glowEnable
	recode::driver.write<int>(base_address + 0x26C, 1);

	recode::driver.write<uint8_t>(vmbase_address + 0x298, id);
	recode::driver.write<int>(base_address + 0x278, 2);  // m_glowEnable
	recode::driver.write<int>(vmbase_address + 0x26C, 1);
}

auto Weapon::update(uintptr_t entity) -> bool
{
	uintptr_t vm = recode::driver.read<uint64_t>(entity + offsets::view_model) & 0xFFFF;
	base_address = recode::driver.read<uintptr_t>(recode::process_image + offsets::entity_list + (vm << 5));
	if (!base_address)
		return false; 

	uintptr_t avm = recode::driver.read<uint64_t>(entity + 0x2e0C) & 0xFFFF; // 0x2e0C is 0x2e00 + 0xC
	vmbase_address = recode::driver.read<uintptr_t>(recode::process_image + offsets::entity_list + (avm << 5));
	if (!vmbase_address)
		return false;

	uintptr_t last_weapon = recode::driver.read<uintptr_t>(entity + offsets::last_active);
	weapon = recode::driver.read<uintptr_t>(recode::process_image + offsets::entity_list + (last_weapon << 5));
	skin_id = recode::driver.read<int>(weapon + offsets::skin_id);
} 
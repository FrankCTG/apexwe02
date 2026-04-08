#pragma once
#include <cstdint>

namespace offsets
{
	constexpr uintptr_t view_render = 0x3F362E0;
	constexpr uintptr_t view_matrix = 0x11a350;

	constexpr uintptr_t local_player = 0x282C358;
	constexpr uintptr_t entity_list = 0x644C0F8;

	constexpr uintptr_t health = 0x324;
	constexpr uintptr_t max_health = 0x468;
	
	constexpr uintptr_t sheild = 0x1a0;
	constexpr uintptr_t max_sheild = 0x1a4;

	constexpr uintptr_t team_id = 0x334;

	constexpr uintptr_t life_state = 0x690;
	constexpr uintptr_t bleedout_state = 0x27c0;

	constexpr uintptr_t bone_array = (0xdb8 + 0x48); // force bone + 0x48

	constexpr uintptr_t last_active = 0x19c4;
	constexpr uintptr_t skin_id = 0xd68;

	constexpr uintptr_t view_model = 0x2e00;
	constexpr uintptr_t model_name = 0x1fa3b90;
}
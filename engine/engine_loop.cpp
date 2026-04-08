#include "engine/engine_loop.h"
#include <engine/player/player.h>
#include <include/includes.h>
#include <engine/impl/offsets.h>
#include <core/systems/visuals/visuals.h>
#include <engine/weapon/weapon.h>

namespace settings
{
	 bool fullbright = false;

	 int weapon_glow_id = 37;
	 bool weapon_glow;

	 bool third_person = false;
	 int third_person_key = 0x04;
	 int third_person_distance = 90;

	 bool no_viewmodel = false;
	 bool no_sky = false ;
	 bool no_fog = false;
	 bool flat_world = false;

	 bool anti_aim = false;
	 bool local_glow = false;
	 float desired_fov = 120.f;

	 bool bloom = false;
	 float bloom_intensity = -100.f;

	 bool aspr = false;	
}

auto engine_loop(std::shared_ptr<PlayerCache> p_cache) -> void
{
	while (true)
	{
		if (settings::aspr)
		{
			recode::driver.write<float>(recode::process_image + 0x1e94be0 + 0x58, 1.333f); // just normal aspect ratio offset
			recode::driver.write<float>(recode::process_image + 0x20ab0a0 + 0x58, 1.333f); // mat_letterbox_aspect_goal 
			recode::driver.write<float>(recode::process_image + 0x20ab700 + 0x58, 0.f); // mat_wide_pillarbox 
			settings::aspr = false;
		}

		if (settings::bloom)
			recode::driver.write<float>(recode::process_image + 0x27045f0 + 0x58, settings::bloom_intensity); // bloom amount override
		else
			recode::driver.write<float>(recode::process_image + 0x27045f0 + 0x58, 1.f); // bloom amount override

		auto local = p_cache->get_local_player();
		if (!local)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		if (settings::local_glow)
			local->set_glow(64);


		Weapon weapon{};
		if (!weapon.update(local->get_entity()))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		if (settings::anti_aim)
		{
			// no idea what offsets you'd call them but i just bruteforced them, fov changer
			float fov_new = settings::desired_fov / 100.0f;
			recode::driver.write<float>(local->get_entity() + 0x4830, fov_new);
			recode::driver.write<float>(local->get_entity() + 0x4834, fov_new);
			recode::driver.write<float>(local->get_entity() + 0x482c, fov_new);
		}
		
		if (settings::weapon_glow)
			weapon.set_glow(settings::weapon_glow_id);

		if (settings::third_person)
			weapon.set_skin(settings::third_person_key); 
		 
		if (settings::fullbright)
			recode::driver.write<int>(recode::process_image + 0x1ead970 + 0x5C , 1); // fullbright
		else
			recode::driver.write<int>(recode::process_image + 0x1ead970 + 0x5C, 0); // fullbright

		if (settings::no_sky)
			recode::driver.write<int>(recode::process_image + 0x2820640 + 0x5C, 32); // no sky
		else
			recode::driver.write<int>(recode::process_image + 0x2820640 + 0x5C, 0); // no sky

		if (GetAsyncKeyState(settings::third_person_key) & 1)
			settings::third_person = !settings::third_person;

		bool ads = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

		if (settings::third_person && !ads)
		{
			recode::driver.write<int>(recode::process_image + 0x26691a0 + 0x5C, settings::third_person_distance); // third person (distance used to do something idk)
		}
		else
		{
			recode::driver.write<int>(recode::process_image + 0x26691a0 + 0x5C, 0);
		}

		auto players = p_cache->get_players(); 
		if (players->empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		for (auto& p : *players)
		{
			if (!p)
				continue;

			if (settings::Glow)
				p->set_glow(settings::glow_id);
		}
	}
}
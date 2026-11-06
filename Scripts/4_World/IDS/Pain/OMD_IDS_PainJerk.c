// One camera jerk at most every cooldown. A miss does not start the cooldown.
// SpawnCameraShake only. Hold breath and the raised weapon are left alone.
class OMD_IDS_PainJerk
{
	protected static float s_NextAt;

	static void Reset()
	{
		s_NextAt = 0;
	}

	static void Try(PlayerBase player)
	{
		float now;
		float chance;
		float cooldown;
		float roll;
		OMD_IDS_PainEffectState fx;

		if (!GetGame())
		{
			return;
		}

		if (GetGame().IsDedicatedServer())
		{
			return;
		}

		if (!player)
		{
			return;
		}

		if (!player.IsControlledPlayer())
		{
			return;
		}

		fx = player.OMD_IDS_GetPainFx();
		if (!fx)
		{
			return;
		}

		chance = fx.GetJerkChance();
		if (chance <= 0)
		{
			return;
		}

		now = GetGame().GetTime();
		if (now < s_NextAt)
		{
			return;
		}

		roll = Math.RandomFloat01();
		if (roll >= chance)
		{
			return;
		}

		cooldown = fx.GetJerkCooldown();
		if (cooldown < 0)
		{
			cooldown = 0;
		}

		s_NextAt = now + (cooldown * 1000);
		if (player.GetCurrentCamera())
		{
			player.GetCurrentCamera().SpawnCameraShake();
		}
	}
};

// Multiplies the vanilla sway weight once, before it is written into the aim model.
// CalculateWeight builds a fresh weight every frame, so the pain factor does not stack.
modded class DayZPlayerImplementAiming
{
	override float CalculateWeight(int stance_index, float current_stamina, float camera_sway_modifier, bool holding_breath)
	{
		float weight;
		float extra;

		weight = super.CalculateWeight(stance_index, current_stamina, camera_sway_modifier, holding_breath);
		extra = 0;
		if (m_PlayerPb)
		{
			if (m_PlayerPb.OMD_IDS_GetPainFx())
			{
				extra = m_PlayerPb.OMD_IDS_GetPainFx().GetSwayAdd();
			}
		}

		if (extra > 0)
		{
			weight = weight * (1 + extra);
		}

		return weight;
	}
};

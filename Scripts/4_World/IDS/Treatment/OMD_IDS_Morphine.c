// Vanilla morphine timer stays. The IDS dose is added on the server after super.
modded class Morphine
{
	override void OnApply(PlayerBase player)
	{
		super.OnApply(player);
		if (!player)
		{
			return;
		}

		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		OMD_IDS_TreatmentHooks.Get().OnAnalgesic(player, "morphine");
	}
};

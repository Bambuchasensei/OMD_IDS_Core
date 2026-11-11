// Vanilla PainKillersMdfr stays, including LIFETIME 240 and ACTIVATION_DELAY 15.
// OnConsume is the moment vanilla enables that modifier. The IDS dose is after super.
modded class PainkillerTablets
{
	override void OnConsume(float amount, PlayerBase consumer)
	{
		super.OnConsume(amount, consumer);
		if (!consumer)
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

		OMD_IDS_TreatmentHooks.Get().OnAnalgesic(consumer, "painkillerTablets");
	}
};

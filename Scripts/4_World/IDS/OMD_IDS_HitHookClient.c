// Empty client socket. OMD_IDS_HitFX overrides OnHitFx.
// Core does not spawn a screen effect.
class OMD_IDS_HitHookClient
{
	void OnHitFx(PlayerBase player, int fxCode, int hitSerial)
	{
	}
};

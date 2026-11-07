// Pain moans share the one vanilla voice slot.
// A new INJURED_* request is dropped while that slot is still busy, so a moan does not cut another voice.
// Hit screams (TAKING_DMG_*) are not filtered here: they keep vanilla priority.
modded class PlayerBase
{
	override bool PlaySoundEventEx(EPlayerSoundEventID id, bool from_anim_system = false, bool is_from_server = false, int param = 0)
	{
		if (OMD_IDS_PainMoanWouldCut(id))
		{
			return false;
		}

		return super.PlaySoundEventEx(id, from_anim_system, is_from_server, param);
	}

	protected bool OMD_IDS_PainMoanWouldCut(EPlayerSoundEventID id)
	{
		PlayerSoundEventHandler handler;
		int currentId;

		if (!OMD_IDS_IsPainMoanId(id))
		{
			return false;
		}

		handler = GetPlayerSoundEventHandler();
		if (!handler)
		{
			return false;
		}

		currentId = handler.GetCurrentStateEventID();
		if (currentId < 0)
		{
			return false;
		}

		return true;
	}

	protected bool OMD_IDS_IsPainMoanId(EPlayerSoundEventID id)
	{
		if (id == EPlayerSoundEventID.INJURED_LIGHT)
		{
			return true;
		}

		if (id == EPlayerSoundEventID.INJURED_MEDIUM)
		{
			return true;
		}

		if (id == EPlayerSoundEventID.INJURED_HIGH)
		{
			return true;
		}

		return false;
	}
};

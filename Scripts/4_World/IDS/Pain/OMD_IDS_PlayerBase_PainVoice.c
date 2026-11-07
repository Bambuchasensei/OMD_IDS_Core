// Pain moans share the one vanilla voice slot.
// A new server-sent INJURED_* request is dropped while a real unfinished voice is playing, so a moan does not cut another voice.
// The silent stamina dummy and a finished state are not treated as busy: vanilla INJURED already outranks that dummy.
// Local vanilla INJURED (PlaySoundEvent, is_from_server false) is not filtered, including when IDS sound is off.
// is_from_server is not an IDS-only flag: ladder/feet bleed also uses SendSoundEvent(INJURED_LIGHT).
// Hit screams (TAKING_DMG_*) are not filtered here: they keep vanilla priority.
modded class PlayerBase
{
	override bool PlaySoundEventEx(EPlayerSoundEventID id, bool from_anim_system = false, bool is_from_server = false, int param = 0)
	{
		if (OMD_IDS_PainMoanWouldCut(id, is_from_server))
		{
			return false;
		}

		return super.PlaySoundEventEx(id, from_anim_system, is_from_server, param);
	}

	protected bool OMD_IDS_PainMoanWouldCut(EPlayerSoundEventID id, bool isFromServer)
	{
		PlayerSoundEventHandler handler;
		PlayerSoundEventBase current;

		if (!isFromServer)
		{
			return false;
		}

		if (!OMD_IDS_IsPainMoanId(id))
		{
			return false;
		}

		handler = GetPlayerSoundEventHandler();
		if (!handler)
		{
			return false;
		}

		current = handler.m_CurrentState;
		if (!current)
		{
			return false;
		}

		if (current.IsDummy())
		{
			return false;
		}

		if (current.IsFinished())
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

// Plays broken_leg_SoundSet when the synced crack counter changes.
// The first value is only remembered, so joining does not crack.
class OMD_IDS_BoneBreakSoundClient
{
	protected bool m_Known;
	protected int m_Seen;
	protected ref EffectSound m_Sound;

	void OnSynced(PlayerBase player, int seq)
	{
		if (!player)
		{
			return;
		}

		if (!GetGame())
		{
			return;
		}

		if (GetGame().IsDedicatedServer())
		{
			return;
		}

		if (!m_Known)
		{
			m_Known = true;
			m_Seen = seq;
			return;
		}

		if (m_Seen == seq)
		{
			return;
		}

		m_Seen = seq;
		player.PlaySoundSet(m_Sound, "broken_leg_SoundSet", 0.1, 0.1);
	}
};

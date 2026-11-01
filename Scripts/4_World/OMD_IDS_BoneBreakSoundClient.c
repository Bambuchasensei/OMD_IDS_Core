// Plays broken_leg_SoundSet when the crack counter changes after the character exists.
// The baseline is the value at creation and the first stream. Joining does not crack.
// A later change is a real fracture and plays. Vanilla plays the same set on the character:
// P:\scripts\4_world\entities\manbase\playerbase.c:4037
class OMD_IDS_BoneBreakSoundClient
{
	protected bool m_Known;
	protected bool m_Live;
	protected int m_Seen;
	protected ref EffectSound m_Sound;

	void Remember(int seq)
	{
		m_Known = true;
		m_Seen = seq;
	}

	void GoLive(int seq)
	{
		m_Known = true;
		m_Seen = seq;
		m_Live = true;
	}

	void OnSynced(PlayerBase player, int seq)
	{
		int oldSeq;
		int knownFlag;
		string knownText;
		string who;
		string line;
		string soundState;
		string okText;
		bool played;

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

		oldSeq = m_Seen;
		knownFlag = 0;
		if (m_Known)
		{
			knownFlag = 1;
		}

		knownText = knownFlag.ToString();
		if (!m_Live)
		{
			if (oldSeq != seq)
			{
				who = PlayerLabel(player);
				line = "[OMD_IDS] client bonebreak seq " + oldSeq.ToString() + "->" + seq.ToString() + " known=" + knownText + " player=" + who;
				PrintFormat("%1", line);
				line = "[OMD_IDS] client bonebreak skipped reason=first stream on appear, baseline remembered";
				PrintFormat("%1", line);
			}

			Remember(seq);
			return;
		}

		if (oldSeq == seq)
		{
			return;
		}

		who = PlayerLabel(player);
		line = "[OMD_IDS] client bonebreak seq " + oldSeq.ToString() + "->" + seq.ToString() + " known=" + knownText + " player=" + who;
		PrintFormat("%1", line);
		m_Seen = seq;
		Print("[OMD_IDS] client bonebreak PlaySoundSet set=broken_leg_SoundSet");
		played = player.PlaySoundSet(m_Sound, "broken_leg_SoundSet", 0.1, 0.1);
		soundState = "null";
		if (m_Sound)
		{
			soundState = "not null";
		}

		okText = "0";
		if (played)
		{
			okText = "1";
		}

		line = "[OMD_IDS] client bonebreak PlaySoundSet result EffectSound=" + soundState + " ok=" + okText;
		PrintFormat("%1", line);
	}

	protected string PlayerLabel(PlayerBase player)
	{
		PlayerIdentity identity;
		string idText;
		string nameText;

		idText = "-";
		nameText = "-";
		if (!player)
		{
			return "-/-";
		}

		identity = player.GetIdentity();
		if (!identity)
		{
			idText = player.GetID().ToString();
			return idText + "/-";
		}

		idText = identity.GetId();
		nameText = identity.GetName();
		if (idText == "")
		{
			idText = "-";
		}

		if (nameText == "")
		{
			nameText = "-";
		}

		return idText + "/" + nameText;
	}
};

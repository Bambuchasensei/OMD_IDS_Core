#ifndef SERVER

modded class MissionGameplay
{
	private bool m_OMD_IDS_SnapshotRequested;

	override void OnInit()
	{
		super.OnInit();
		OMD_IDS_HudManager.Create();
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		OMD_IDS_HudManager manager = OMD_IDS_HudManager.Get();
		if (!manager)
		{
			return;
		}

		RequestSnapshotOnce();
		OMD_IDS_InjuryMailbox mailbox = OMD_IDS_InjuryMailbox.Get();
		if (!mailbox)
		{
			return;
		}

		if (!mailbox.HasPending())
		{
			return;
		}

		if (!LocalPlayerReady())
		{
			return;
		}

		array<ref OMD_IDS_InjurySnapshotEntry> snapshot = mailbox.Take();
		array<ref OMD_IDS_HudDamageEntry> entries = OMD_IDS_InjuryHudBridge.ToEntries(snapshot);
		manager.SetActiveDamages(entries);
	}

	override void OnMissionFinish()
	{
		OMD_IDS_InjuryMailbox mailbox = OMD_IDS_InjuryMailbox.Get();
		if (mailbox)
		{
			mailbox.Clear();
		}

		m_OMD_IDS_SnapshotRequested = false;
		OMD_IDS_HudManager.Destroy();
		super.OnMissionFinish();
	}

	private void RequestSnapshotOnce()
	{
		PlayerBase player;
		ScriptRPC rpc;

		if (m_OMD_IDS_SnapshotRequested)
		{
			return;
		}

		if (!LocalPlayerReady())
		{
			return;
		}

		if (!Class.CastTo(player, GetGame().GetPlayer()))
		{
			return;
		}

		rpc = new ScriptRPC();
		rpc.Send(player, OMD_IDS_Rpc.SNAPSHOT_REQUEST, true, null);
		m_OMD_IDS_SnapshotRequested = true;
	}

	private bool LocalPlayerReady()
	{
		PlayerBase player;

		if (!GetGame())
		{
			return false;
		}

		if (!Class.CastTo(player, GetGame().GetPlayer()))
		{
			return false;
		}

		if (!player.IsControlledPlayer())
		{
			return false;
		}

		return true;
	}
};

#endif

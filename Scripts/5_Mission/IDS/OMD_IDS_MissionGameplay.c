#ifndef SERVER

modded class MissionGameplay
{
	private bool m_OMD_IDS_SnapshotRequested;
	private bool m_OMD_IDS_WasRaised;

	override void OnInit()
	{
		super.OnInit();
		OMD_IDS_HudManager.Create();
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		OMD_IDS_WatchAim();
		OMD_IDS_HudManager manager = OMD_IDS_HudManager.Get();
		if (!manager)
		{
			return;
		}

		RequestSnapshotOnce();
		manager.UpdatePain(timeslice);
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
		OMD_IDS_PainStore painStore;
		if (mailbox)
		{
			mailbox.Clear();
		}

		painStore = OMD_IDS_PainStore.Get();
		if (painStore)
		{
			painStore.Replace(0, null, null);
		}

		OMD_IDS_ClearPainFx();

		m_OMD_IDS_SnapshotRequested = false;
		m_OMD_IDS_WasRaised = false;
		OMD_IDS_HudManager.Destroy();
		super.OnMissionFinish();
	}

	private void OMD_IDS_WatchAim()
	{
		PlayerBase player;
		bool raised;

		if (!GetGame())
		{
			return;
		}

		if (!Class.CastTo(player, GetGame().GetPlayer()))
		{
			m_OMD_IDS_WasRaised = false;
			return;
		}

		raised = player.IsFireWeaponRaised();
		if (raised)
		{
			if (!m_OMD_IDS_WasRaised)
			{
				OMD_IDS_PainJerk.Try(player);
			}
		}

		m_OMD_IDS_WasRaised = raised;
	}

	private void OMD_IDS_ClearPainFx()
	{
		PlayerBase player;

		if (!GetGame())
		{
			return;
		}

		if (!Class.CastTo(player, GetGame().GetPlayer()))
		{
			return;
		}

		if (!player.OMD_IDS_GetPainFx())
		{
			return;
		}

		player.OMD_IDS_GetPainFx().Clear();
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

#ifndef SERVER

modded class MissionGameplay
{
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

		manager.OnUpdate(timeslice);
		OMD_IDS_InjuryMailbox mailbox = OMD_IDS_InjuryMailbox.Get();
		if (!mailbox)
		{
			return;
		}

		if (!mailbox.HasPending())
		{
			return;
		}

		array<ref OMD_IDS_InjurySnapshotEntry> snapshot = mailbox.Take();
		array<ref OMD_IDS_HudDamageEntry> entries = OMD_IDS_InjuryHudBridge.ToEntries(snapshot);
		manager.SetActiveDamages(entries);
	}

	override void OnMissionFinish()
	{
		OMD_IDS_HudManager.Destroy();
		super.OnMissionFinish();
	}
};

#endif

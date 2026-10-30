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
	}

	override void OnMissionFinish()
	{
		OMD_IDS_HudManager.Destroy();
		super.OnMissionFinish();
	}
};

#endif

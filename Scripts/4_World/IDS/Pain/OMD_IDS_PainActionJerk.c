modded class ActionContinuousBase
{
	override void OnStartClient(ActionData action_data)
	{
		super.OnStartClient(action_data);
		if (!action_data)
		{
			return;
		}

		OMD_IDS_PainJerk.Try(action_data.m_Player);
	}
};

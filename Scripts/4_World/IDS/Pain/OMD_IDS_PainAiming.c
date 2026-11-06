// Adds a fraction of the vanilla aim sway. The added fraction is already clamped.
modded class DayZPlayerImplementAiming
{
	override bool ProcessAimFilters(float pDt, SDayZPlayerAimingModel pModel, int stance_index)
	{
		bool ok;
		float extra;

		ok = super.ProcessAimFilters(pDt, pModel, stance_index);
		extra = 0;
		if (m_PlayerPb)
		{
			if (m_PlayerPb.OMD_IDS_GetPainFx())
			{
				extra = m_PlayerPb.OMD_IDS_GetPainFx().GetSwayAdd();
			}
		}

		if (extra > 0)
		{
			m_SwayWeight = m_SwayWeight * (1 + extra);
		}

		return ok;
	}

	void OMD_IDS_ResetBreathHold()
	{
		m_HoldingBreathSet = false;
	}
};

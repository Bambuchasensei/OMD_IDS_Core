// One requester for the whole session. MissionGameplay starts it and stops it.
// ChromAber above the vanilla material max is clamped by the engine to 0.05.
class PPERequester_OMD_IDS_Pain extends PPERequester_GameplayBase
{
	static const int LAYER = 1200;

	protected float m_Phase;
	protected float m_ShownChrom;
	protected float m_ShownGauss;

	override void OnUpdate(float delta)
	{
		float step;
		float sine;
		float chromLo;
		float chromHi;
		float gaussLo;
		float gaussHi;
		float targetChrom;
		float targetGauss;
		float blend;
		PlayerBase player;
		OMD_IDS_PainEffectState fx;

		super.OnUpdate(delta);
		step = 0.5;
		chromLo = 0;
		chromHi = 0;
		gaussLo = 0;
		gaussHi = 0;
		if (GetGame())
		{
			if (Class.CastTo(player, GetGame().GetPlayer()))
			{
				fx = player.OMD_IDS_GetPainFx();
				if (fx)
				{
					step = fx.GetWaveStep();
					chromLo = fx.GetChromMin();
					chromHi = fx.GetChromMax();
					gaussLo = fx.GetGaussMin();
					gaussHi = fx.GetGaussMax();
				}
			}
		}

		if (step <= 0)
		{
			step = 0.5;
		}

		m_Phase = m_Phase + (delta * step);
		if (m_Phase >= Math.PI2)
		{
			m_Phase = 0;
		}

		sine = Math.Sin(m_Phase);
		sine = (sine + 1) * 0.5;
		targetChrom = chromLo + ((chromHi - chromLo) * sine);
		targetGauss = gaussLo + ((gaussHi - gaussLo) * sine);
		blend = delta * 2;
		if (blend > 1)
		{
			blend = 1;
		}

		m_ShownChrom = m_ShownChrom + ((targetChrom - m_ShownChrom) * blend);
		m_ShownGauss = m_ShownGauss + ((targetGauss - m_ShownGauss) * blend);
		SetTargetValueFloat(PostProcessEffectType.ChromAber, PPEChromAber.PARAM_POWERX, false, m_ShownChrom, LAYER, PPOperators.HIGHEST);
		SetTargetValueFloat(PostProcessEffectType.ChromAber, PPEChromAber.PARAM_POWERY, false, m_ShownChrom, LAYER, PPOperators.HIGHEST);
		SetTargetValueFloat(PostProcessEffectType.GaussFilter, PPEGaussFilter.PARAM_INTENSITY, false, m_ShownGauss, LAYER, PPOperators.HIGHEST);
	}

	bool OMD_IDS_IsFaded()
	{
		if (m_ShownChrom > 0.001)
		{
			return false;
		}

		if (m_ShownGauss > 0.001)
		{
			return false;
		}

		return true;
	}

	override protected void OnStop(Param par = null)
	{
		super.OnStop(par);
		m_Phase = 0;
		m_ShownChrom = 0;
		m_ShownGauss = 0;
	}
};

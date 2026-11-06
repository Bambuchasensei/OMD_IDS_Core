// One requester for the whole session. MissionGameplay starts it, feeds the numbers, and stops it.
// ChromAber above the vanilla material max is clamped by the engine to 0.05.
class PPERequester_OMD_IDS_Pain extends PPERequester_GameplayBase
{
	static const int LAYER = 1200;

	protected float m_ChromMin;
	protected float m_ChromMax;
	protected float m_GaussMin;
	protected float m_GaussMax;
	protected float m_WaveStep;
	protected float m_Phase;
	protected float m_ShownChrom;
	protected float m_ShownGauss;

	void SetVisual(float chromMin, float chromMax, float gaussMin, float gaussMax, float waveStep)
	{
		m_ChromMin = chromMin;
		m_ChromMax = chromMax;
		m_GaussMin = gaussMin;
		m_GaussMax = gaussMax;
		m_WaveStep = waveStep;
	}

	override void OnUpdate(float delta)
	{
		float step;
		float sine;
		float targetChrom;
		float targetGauss;
		float blend;

		super.OnUpdate(delta);
		step = m_WaveStep;
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
		targetChrom = m_ChromMin + ((m_ChromMax - m_ChromMin) * sine);
		targetGauss = m_GaussMin + ((m_GaussMax - m_GaussMin) * sine);
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
		m_ChromMin = 0;
		m_ChromMax = 0;
		m_GaussMin = 0;
		m_GaussMax = 0;
		m_WaveStep = 0;
		m_Phase = 0;
		m_ShownChrom = 0;
		m_ShownGauss = 0;
	}
};

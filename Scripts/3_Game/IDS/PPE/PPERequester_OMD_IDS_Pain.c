// Один requester на сессию. Миссия включает его, отдаёт числа и останавливает.
// ChromAber выше ванильного потолка материала движок зажимает в 0.05.
// SetVisual снова включает updating: ванильный OnUpdate сам его выключает.
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
	protected bool m_DebugEnabled;
	protected bool m_DebugVisualPending;
	protected bool m_DebugFadedNoted;
	protected bool m_DebugFlushPending;

	void OMD_IDS_SetDebugEnabled(bool enabled)
	{
		m_DebugEnabled = enabled;
	}

	void SetVisual(float chromMin, float chromMax, float gaussMin, float gaussMax, float waveStep)
	{
		bool changed;

		changed = false;
		if (m_ChromMin != chromMin)
		{
			changed = true;
		}

		if (m_ChromMax != chromMax)
		{
			changed = true;
		}

		if (m_GaussMin != gaussMin)
		{
			changed = true;
		}

		if (m_GaussMax != gaussMax)
		{
			changed = true;
		}

		if (m_WaveStep != waveStep)
		{
			changed = true;
		}

		m_ChromMin = chromMin;
		m_ChromMax = chromMax;
		m_GaussMin = gaussMin;
		m_GaussMax = gaussMax;
		m_WaveStep = waveStep;
		SetRequesterUpdating(true);
		if (!changed)
		{
			return;
		}

		m_DebugVisualPending = true;
		m_DebugFadedNoted = false;
		OMD_IDS_TraceRequester("set");
	}

	override void OnUpdate(float delta)
	{
		float step;
		float sine;
		float targetChrom;
		float targetGauss;
		float blend;
		bool targetsClear;

		super.OnUpdate(delta);
		if (!m_IsRunning)
		{
			if (m_DebugFlushPending)
			{
				m_DebugFlushPending = false;
				OMD_IDS_TraceRequester("flush");
			}

			return;
		}

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
		targetsClear = false;
		if (m_ChromMax <= 0)
		{
			if (m_GaussMax <= 0)
			{
				targetsClear = true;
			}
		}

		if (targetsClear)
		{
			if (m_ShownChrom < 0.001)
			{
				if (m_ShownGauss < 0.001)
				{
					m_ShownChrom = 0;
					m_ShownGauss = 0;
				}
			}
		}

		SetTargetValueFloat(PostProcessEffectType.ChromAber, PPEChromAber.PARAM_POWERX, false, m_ShownChrom, LAYER, PPOperators.HIGHEST);
		SetTargetValueFloat(PostProcessEffectType.ChromAber, PPEChromAber.PARAM_POWERY, false, m_ShownChrom, LAYER, PPOperators.HIGHEST);
		SetTargetValueFloat(PostProcessEffectType.GaussFilter, PPEGaussFilter.PARAM_INTENSITY, false, m_ShownGauss, LAYER, PPOperators.HIGHEST);
		// super уже выключил updating. Пока идём, включаем снова, иначе волна и спад застывают.
		SetRequesterUpdating(true);
		if (m_DebugVisualPending)
		{
			m_DebugVisualPending = false;
			OMD_IDS_TraceRequester("update");
		}

		if (!targetsClear)
		{
			return;
		}

		if (!OMD_IDS_IsFaded())
		{
			return;
		}

		if (m_DebugFadedNoted)
		{
			return;
		}

		m_DebugFadedNoted = true;
		OMD_IDS_TraceRequester("faded");
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
		// Ваниль: сначала updating, потом default всем активным параметрам.
		// Повторный default на три канала не снимает этот флаг.
		// SetTargetValueFloat(0) после super как раз снял бы его, поэтому здесь его нет.
		super.OnStop(par);
		m_ChromMin = 0;
		m_ChromMax = 0;
		m_GaussMin = 0;
		m_GaussMax = 0;
		m_WaveStep = 0;
		m_Phase = 0;
		m_ShownChrom = 0;
		m_ShownGauss = 0;
		m_DebugVisualPending = false;
		SetTargetValueFloatDefault(PostProcessEffectType.ChromAber, PPEChromAber.PARAM_POWERX);
		SetTargetValueFloatDefault(PostProcessEffectType.ChromAber, PPEChromAber.PARAM_POWERY);
		SetTargetValueFloatDefault(PostProcessEffectType.GaussFilter, PPEGaussFilter.PARAM_INTENSITY);
		m_DebugFlushPending = true;
		OMD_IDS_TraceRequester("stop");
	}

	protected void OMD_IDS_TraceRequester(string step)
	{
		if (!m_DebugEnabled)
		{
			return;
		}

		Print(string.Format("[OMD_IDS][Pain][PPE] %1 chrom=%2..%3 gauss=%4..%5 wave=%6 shown=%7/%8", step, m_ChromMin, m_ChromMax, m_GaussMin, m_GaussMax, m_WaveStep, m_ShownChrom, m_ShownGauss));
	}
};

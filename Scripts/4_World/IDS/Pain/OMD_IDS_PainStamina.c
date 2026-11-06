// Extra hold-breath drain. The server is authoritative; the same multiplier
// is applied here so the owner client predicts the same drain.
modded class StaminaHandler
{
	override void DepleteStaminaEx(EStaminaModifiers modifier, float dT = -1, float coef = 1.0)
	{
		float before;
		float added;
		float extra;

		before = m_StaminaDepletion;
		super.DepleteStaminaEx(modifier, dT, coef);
		extra = 0;
		if (modifier == EStaminaModifiers.HOLD_BREATH)
		{
			extra = BreathExtra();
		}

		if (modifier == EStaminaModifiers.MELEE_HEAVY)
		{
			extra = PowerExtra();
		}

		if (modifier == EStaminaModifiers.PUSH_CAR)
		{
			extra = PowerExtra();
		}

		if (extra == 0)
		{
			return;
		}

		added = m_StaminaDepletion - before;
		m_StaminaDepletion = before + (added * (1 + extra));
	}

	protected float BreathExtra()
	{
		OMD_IDS_PainEffectState fx;

		if (!m_Player)
		{
			return 0;
		}

		fx = m_Player.OMD_IDS_GetPainFx();
		if (!fx)
		{
			return 0;
		}

		return fx.GetBreathAdd();
	}

	protected float PowerExtra()
	{
		OMD_IDS_PainEffectState fx;

		if (!m_Player)
		{
			return 0;
		}

		fx = m_Player.OMD_IDS_GetPainFx();
		if (!fx)
		{
			return 0;
		}

		return fx.GetPowerAdd();
	}

	override void ProcessStaminaChanges(float deltaT, bool isServerOrSingleplayer)
	{
		float saved;
		float extra;
		float scaled;

		saved = m_StaminaRecoveryMultiplier;
		extra = RecoveryExtra();
		scaled = saved * (1 + extra);
		if (scaled < 0)
		{
			scaled = 0;
		}

		m_StaminaRecoveryMultiplier = scaled;
		super.ProcessStaminaChanges(deltaT, isServerOrSingleplayer);
		m_StaminaRecoveryMultiplier = saved;
	}

	protected float RecoveryExtra()
	{
		OMD_IDS_PainEffectState fx;

		if (!m_Player)
		{
			return 0;
		}

		fx = m_Player.OMD_IDS_GetPainFx();
		if (!fx)
		{
			return 0;
		}

		return fx.GetRecoveryAdd();
	}
};

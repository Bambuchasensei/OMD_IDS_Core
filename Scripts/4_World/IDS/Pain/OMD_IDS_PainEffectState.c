// Numbers the server already multiplied by painEffectScale.
// The client copies them from PAIN_LEVELS and does not read injuries.json.
class OMD_IDS_PainEffectState
{
	protected float m_BreathAdd;
	protected float m_RecoveryAdd;
	protected float m_PowerAdd;
	protected float m_SwayAdd;
	protected float m_JerkChance;
	protected float m_JerkCooldown;
	protected float m_ActionAdd;
	protected float m_ChromMin;
	protected float m_ChromMax;
	protected float m_GaussMin;
	protected float m_GaussMax;
	protected float m_WaveStep;

	void OMD_IDS_PainEffectState()
	{
		Clear();
	}

	void Clear()
	{
		m_BreathAdd = 0;
		m_RecoveryAdd = 0;
		m_PowerAdd = 0;
		m_SwayAdd = 0;
		m_JerkChance = 0;
		m_JerkCooldown = 5;
		m_ActionAdd = 0;
		m_ChromMin = 0;
		m_ChromMax = 0;
		m_GaussMin = 0;
		m_GaussMax = 0;
		m_WaveStep = 0.5;
	}

	void SetBreathAdd(float value)
	{
		m_BreathAdd = value;
	}

	float GetBreathAdd()
	{
		return m_BreathAdd;
	}

	void SetRecoveryAdd(float value)
	{
		m_RecoveryAdd = value;
	}

	float GetRecoveryAdd()
	{
		return m_RecoveryAdd;
	}

	void SetPowerAdd(float value)
	{
		m_PowerAdd = value;
	}

	float GetPowerAdd()
	{
		return m_PowerAdd;
	}

	void SetSwayAdd(float value)
	{
		m_SwayAdd = value;
	}

	float GetSwayAdd()
	{
		return m_SwayAdd;
	}

	void SetJerkChance(float value)
	{
		m_JerkChance = value;
	}

	float GetJerkChance()
	{
		return m_JerkChance;
	}

	void SetJerkCooldown(float value)
	{
		m_JerkCooldown = value;
	}

	float GetJerkCooldown()
	{
		return m_JerkCooldown;
	}

	void SetActionAdd(float value)
	{
		m_ActionAdd = value;
	}

	float GetActionAdd()
	{
		return m_ActionAdd;
	}

	void SetVisual(float chromMin, float chromMax, float gaussMin, float gaussMax, float waveStep)
	{
		m_ChromMin = chromMin;
		m_ChromMax = chromMax;
		m_GaussMin = gaussMin;
		m_GaussMax = gaussMax;
		m_WaveStep = waveStep;
	}

	float GetChromMin()
	{
		return m_ChromMin;
	}

	float GetChromMax()
	{
		return m_ChromMax;
	}

	float GetGaussMin()
	{
		return m_GaussMin;
	}

	float GetGaussMax()
	{
		return m_GaussMax;
	}

	float GetWaveStep()
	{
		return m_WaveStep;
	}

	void WriteRpc(ScriptRPC rpc)
	{
		rpc.Write(m_BreathAdd);
		rpc.Write(m_RecoveryAdd);
		rpc.Write(m_PowerAdd);
		rpc.Write(m_SwayAdd);
		rpc.Write(m_JerkChance);
		rpc.Write(m_JerkCooldown);
		rpc.Write(m_ActionAdd);
		rpc.Write(m_ChromMin);
		rpc.Write(m_ChromMax);
		rpc.Write(m_GaussMin);
		rpc.Write(m_GaussMax);
		rpc.Write(m_WaveStep);
	}

	bool ReadRpc(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_BreathAdd))
		{
			return false;
		}

		if (!ctx.Read(m_RecoveryAdd))
		{
			return false;
		}

		if (!ctx.Read(m_PowerAdd))
		{
			return false;
		}

		if (!ctx.Read(m_SwayAdd))
		{
			return false;
		}

		if (!ctx.Read(m_JerkChance))
		{
			return false;
		}

		if (!ctx.Read(m_JerkCooldown))
		{
			return false;
		}

		if (!ctx.Read(m_ActionAdd))
		{
			return false;
		}

		if (!ctx.Read(m_ChromMin))
		{
			return false;
		}

		if (!ctx.Read(m_ChromMax))
		{
			return false;
		}

		if (!ctx.Read(m_GaussMin))
		{
			return false;
		}

		if (!ctx.Read(m_GaussMax))
		{
			return false;
		}

		if (!ctx.Read(m_WaveStep))
		{
			return false;
		}

		return true;
	}
};

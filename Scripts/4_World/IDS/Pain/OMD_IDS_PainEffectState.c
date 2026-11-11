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
	protected float m_HighJerkStrength;
	protected float m_HighJerkInterval;
	// PAIN_LEVELS tail only. Not saved and not NetSync. 0 means no hit effect.
	protected int m_HitFxCode;
	protected int m_HitSerial;

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
		m_HighJerkStrength = 0;
		m_HighJerkInterval = 0;
		m_HitFxCode = 0;
		m_HitSerial = 0;
	}

	void CopyFrom(OMD_IDS_PainEffectState other)
	{
		if (!other)
		{
			Clear();
			return;
		}

		m_BreathAdd = other.m_BreathAdd;
		m_RecoveryAdd = other.m_RecoveryAdd;
		m_PowerAdd = other.m_PowerAdd;
		m_SwayAdd = other.m_SwayAdd;
		m_JerkChance = other.m_JerkChance;
		m_JerkCooldown = other.m_JerkCooldown;
		m_ActionAdd = other.m_ActionAdd;
		m_ChromMin = other.m_ChromMin;
		m_ChromMax = other.m_ChromMax;
		m_GaussMin = other.m_GaussMin;
		m_GaussMax = other.m_GaussMax;
		m_WaveStep = other.m_WaveStep;
		m_HighJerkStrength = other.m_HighJerkStrength;
		m_HighJerkInterval = other.m_HighJerkInterval;
		m_HitFxCode = other.m_HitFxCode;
		m_HitSerial = other.m_HitSerial;
	}

	bool Matches(OMD_IDS_PainEffectState other)
	{
		if (!other)
		{
			return false;
		}

		if (!Near(m_BreathAdd, other.m_BreathAdd))
		{
			return false;
		}

		if (!Near(m_RecoveryAdd, other.m_RecoveryAdd))
		{
			return false;
		}

		if (!Near(m_PowerAdd, other.m_PowerAdd))
		{
			return false;
		}

		if (!Near(m_SwayAdd, other.m_SwayAdd))
		{
			return false;
		}

		if (!Near(m_JerkChance, other.m_JerkChance))
		{
			return false;
		}

		if (!Near(m_JerkCooldown, other.m_JerkCooldown))
		{
			return false;
		}

		if (!Near(m_ActionAdd, other.m_ActionAdd))
		{
			return false;
		}

		if (!Near(m_ChromMin, other.m_ChromMin))
		{
			return false;
		}

		if (!Near(m_ChromMax, other.m_ChromMax))
		{
			return false;
		}

		if (!Near(m_GaussMin, other.m_GaussMin))
		{
			return false;
		}

		if (!Near(m_GaussMax, other.m_GaussMax))
		{
			return false;
		}

		if (!Near(m_WaveStep, other.m_WaveStep))
		{
			return false;
		}

		if (!Near(m_HighJerkStrength, other.m_HighJerkStrength))
		{
			return false;
		}

		if (!Near(m_HighJerkInterval, other.m_HighJerkInterval))
		{
			return false;
		}

		return true;
	}

	protected bool Near(float a, float b)
	{
		float delta;

		delta = a - b;
		if (delta < 0)
		{
			delta = -delta;
		}

		if (delta > 0.0001)
		{
			return false;
		}

		return true;
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

	void SetHighJerk(float strength, float intervalSec)
	{
		m_HighJerkStrength = strength;
		m_HighJerkInterval = intervalSec;
	}

	float GetHighJerkStrength()
	{
		return m_HighJerkStrength;
	}

	float GetHighJerkInterval()
	{
		return m_HighJerkInterval;
	}

	int GetHitFxCode()
	{
		return m_HitFxCode;
	}

	int GetHitSerial()
	{
		return m_HitSerial;
	}

	void StoreHitFx(int fxCode, int hitSerial)
	{
		m_HitFxCode = fxCode;
		m_HitSerial = hitSerial;
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
		rpc.Write(m_HighJerkStrength);
		rpc.Write(m_HighJerkInterval);
		rpc.Write(m_HitFxCode);
		rpc.Write(m_HitSerial);
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

		if (!ctx.Read(m_HighJerkStrength))
		{
			m_HighJerkStrength = 0;
			m_HighJerkInterval = 0;
			m_HitFxCode = 0;
			m_HitSerial = 0;
			return true;
		}

		if (!ctx.Read(m_HighJerkInterval))
		{
			m_HighJerkStrength = 0;
			m_HighJerkInterval = 0;
			m_HitFxCode = 0;
			m_HitSerial = 0;
			return true;
		}

		if (!ctx.Read(m_HitFxCode))
		{
			m_HitFxCode = 0;
			m_HitSerial = 0;
			return true;
		}

		if (!ctx.Read(m_HitSerial))
		{
			m_HitFxCode = 0;
			m_HitSerial = 0;
			return true;
		}

		return true;
	}
};

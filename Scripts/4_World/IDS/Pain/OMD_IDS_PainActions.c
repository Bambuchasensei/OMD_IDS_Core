// Lengthens the script timer of precise work. Weapon reload stays on the animation.
class OMD_IDS_PainActions
{
	static float Extra(ActionData action_data)
	{
		PlayerBase player;
		OMD_IDS_PainEffectState fx;

		if (!action_data)
		{
			return 0;
		}

		if (!IsPrecise(action_data.m_Action))
		{
			return 0;
		}

		player = action_data.m_Player;
		if (!player)
		{
			return 0;
		}

		fx = player.OMD_IDS_GetPainFx();
		if (!fx)
		{
			return 0;
		}

		return fx.GetActionAdd();
	}

	static bool IsPrecise(ActionBase action)
	{
		string name;

		if (!action)
		{
			return false;
		}

		if (action.IsInherited(ActionBandageBase))
		{
			return true;
		}

		if (action.IsInherited(ActionLoadMagazine))
		{
			return true;
		}

		if (action.IsInherited(ActionLoadMagazineQuick))
		{
			return true;
		}

		if (action.IsInherited(ActionDefibrilateBase))
		{
			return true;
		}

		if (action.IsInherited(ActionCPR))
		{
			return true;
		}

		if (action.IsInherited(ActionBurnSewSelf))
		{
			return true;
		}

		if (action.IsInherited(ActionBurnSewTarget))
		{
			return true;
		}

		if (action.IsInherited(ActionSplintSelf))
		{
			return true;
		}

		if (action.IsInherited(ActionSplintTarget))
		{
			return true;
		}

		if (action.IsInherited(ActionCollectBloodSelf))
		{
			return true;
		}

		if (action.IsInherited(ActionCollectBloodTarget))
		{
			return true;
		}

		if (action.IsInherited(ActionGiveSalineSelf))
		{
			return true;
		}

		if (action.IsInherited(ActionGiveSalineTarget))
		{
			return true;
		}

		if (action.IsInherited(ActionGiveBloodSelf))
		{
			return true;
		}

		if (action.IsInherited(ActionGiveBloodTarget))
		{
			return true;
		}

		if (action.IsInherited(ActionTestBloodSelf))
		{
			return true;
		}

		if (action.IsInherited(ActionTestBloodTarget))
		{
			return true;
		}

		if (action.IsInherited(ActionCheckPulseTarget))
		{
			return true;
		}

		if (action.IsInherited(ActionCollectSampleSelf))
		{
			return true;
		}

		if (action.IsInherited(ActionCollectSampleTarget))
		{
			return true;
		}

		if (action.IsInherited(ActionMeasureTemperatureSelf))
		{
			return true;
		}

		if (action.IsInherited(ActionMeasureTemperatureTarget))
		{
			return true;
		}

		name = action.ClassName();
		if (name.IndexOf("ActionRepair") == 0)
		{
			return true;
		}

		return false;
	}
};

modded class CAContinuousTime
{
	override void Setup(ActionData action_data)
	{
		float extra;

		super.Setup(action_data);
		extra = OMD_IDS_PainActions.Extra(action_data);
		if (extra <= 0)
		{
			return;
		}

		if (m_AdjustedTimeToComplete <= 0)
		{
			return;
		}

		m_AdjustedTimeToComplete = m_AdjustedTimeToComplete * (1 + extra);
	}
};

modded class CAContinuousRepeat
{
	override void Setup(ActionData action_data)
	{
		float extra;

		super.Setup(action_data);
		extra = OMD_IDS_PainActions.Extra(action_data);
		if (extra <= 0)
		{
			return;
		}

		if (m_TimeToComplete <= 0)
		{
			return;
		}

		m_TimeToComplete = m_TimeToComplete * (1 + extra);
	}
};

// Blood transfer spends quantity, not CAContinuousTime. Slow the rate once per action.
// Setup runs again after every chunk, so the rate must not be divided twice.
modded class CAContinuousQuantityBloodTransfer
{
	protected bool m_OMD_IDS_RateScaled;

	override void Setup(ActionData action_data)
	{
		float extra;

		super.Setup(action_data);
		if (m_OMD_IDS_RateScaled)
		{
			return;
		}

		extra = OMD_IDS_PainActions.Extra(action_data);
		if (extra <= 0)
		{
			return;
		}

		if (m_QuantityUsedPerSecond <= 0)
		{
			return;
		}

		m_QuantityUsedPerSecond = m_QuantityUsedPerSecond / (1 + extra);
		m_OMD_IDS_RateScaled = true;
	}
};

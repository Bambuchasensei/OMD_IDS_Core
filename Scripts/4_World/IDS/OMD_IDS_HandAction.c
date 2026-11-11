// The shock flag has to live on the ActionData every continuous action already has.
// OMD_IDS_HandActionData is only the suture and the bullet extraction.
modded class ActionData
{
	int m_OMD_IDS_HandShockDone;
	float m_OMD_IDS_HandAppliedMult;
};

// AnimatedActionBase.Start calls OnStartServer before it creates the component.
// The component exists only after super.Start returns. Shock stays once per ActionData.
modded class ActionContinuousBase
{
	override void Start(ActionData action_data)
	{
		super.Start(action_data);
		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		if (!action_data)
		{
			return;
		}

		if (action_data.m_OMD_IDS_HandShockDone != 0)
		{
			return;
		}

		if (OMD_IDS_HandAction.IsHandTimed(action_data) == 0)
		{
			return;
		}

		action_data.m_OMD_IDS_HandShockDone = 1;
		OMD_IDS_TreatmentHooks.Get().OnHandActionStart(action_data);
	}
};

class OMD_IDS_HandAction
{
	static int IsHandTimed(ActionData action_data)
	{
		if (!action_data)
		{
			return 0;
		}

		if (!action_data.m_ActionComponent)
		{
			return 0;
		}

		if (IsSkipped(action_data.m_Action) != 0)
		{
			return 0;
		}

		if (action_data.m_ActionComponent.IsInherited(CAContinuousQuantityEdible))
		{
			return 1;
		}

		if (action_data.m_ActionComponent.IsInherited(CAContinuousRepeat))
		{
			return 1;
		}

		if (action_data.m_ActionComponent.IsInherited(CAContinuousCraft))
		{
			return 1;
		}

		if (action_data.m_ActionComponent.IsInherited(CAContinuousTime))
		{
			return 1;
		}

		return 0;
	}

	static float AppliedMult(ActionData action_data)
	{
		float leftLoad;
		float rightLoad;
		float configured;
		PlayerBase player;

		leftLoad = 0;
		rightLoad = 0;
		configured = OMD_IDS_HandActionLoad.FactoryMult();
		if (!action_data)
		{
			return 1;
		}

		player = action_data.m_Player;
		OMD_IDS_TreatmentHooks.Get().HandActionLoads(player, leftLoad, rightLoad, configured);
		return OMD_IDS_HandActionLoad.TimeMult(leftLoad, rightLoad, configured);
	}

	// Bottle, pond and well drinks share these components with food and repeats.
	// Vitamin feed is medicine. They stay on the vanilla timer.
	private static int IsSkipped(ActionBase action)
	{
		if (!action)
		{
			return 0;
		}

		if (action.IsInherited(ActionDrink))
		{
			return 1;
		}

		if (action.IsInherited(ActionForceDrink))
		{
			return 1;
		}

		if (action.IsInherited(ActionDrinkPondContinuous))
		{
			return 1;
		}

		if (action.IsInherited(ActionDrinkWellContinuous))
		{
			return 1;
		}

		if (action.IsInherited(ActionFeedVitaminBottle))
		{
			return 1;
		}

		return 0;
	}
};

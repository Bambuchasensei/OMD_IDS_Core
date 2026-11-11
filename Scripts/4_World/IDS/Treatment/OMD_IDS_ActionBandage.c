// Hand bandage. Vanilla ActionBandageSelf and ActionBandageTarget stay the actions.
// Time and quantity stay on ActionBandageSelfCB / ActionBandageTargetCB and ApplyBandage.
// One finish spends one portion, then m_Spent stops the repeat.
class OMD_IDS_BandageWork
{
	static void Finish(ActionData action_data, int selfUse)
	{
		int zoneId;
		int variantId;
		PlayerBase actor;
		PlayerBase patient;
		ItemBase item;
		ActionBandageBase bandageAction;
		OMD_IDS_HandActionData data;
		OMD_IDS_TreatmentHooks hooks;

		if (!action_data)
		{
			return;
		}

		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		actor = action_data.m_Player;
		patient = actor;
		if (selfUse == 0)
		{
			patient = OMD_IDS_HandSnapshot.OtherPlayer(action_data.m_Target);
		}

		if (!actor)
		{
			return;
		}

		if (!patient)
		{
			return;
		}

		if (!patient.IsAlive())
		{
			return;
		}

		if (selfUse == 0)
		{
			if (action_data.m_Action)
			{
				if (!action_data.m_Action.CanReceiveAction(action_data.m_Target))
				{
					return;
				}
			}
		}

		variantId = 0;
		data = OMD_IDS_HandActionData.Cast(action_data);
		if (data)
		{
			variantId = data.m_HandVariant;
		}

		hooks = OMD_IDS_TreatmentHooks.Get();
		zoneId = hooks.BandageZone(patient, variantId);
		if (zoneId == 0)
		{
			hooks.OnTreatRefused(actor, patient, zoneId, OMD_IDS_TreatmentHooks.OP_BANDAGE);
			return;
		}

		if (!hooks.CanBandageServer(patient, zoneId))
		{
			hooks.OnTreatRefused(actor, patient, zoneId, OMD_IDS_TreatmentHooks.OP_BANDAGE);
			return;
		}

		item = action_data.m_MainItem;
		if (!item)
		{
			return;
		}

		hooks.OnBandage(actor, patient, zoneId, item);
		if (!Class.CastTo(bandageAction, action_data.m_Action))
		{
			return;
		}

		bandageAction.ApplyBandage(item, patient);
		if (data)
		{
			data.m_Spent = 1;
		}
	}

	static bool Continue(ActionData action_data)
	{
		OMD_IDS_HandActionData data;

		data = OMD_IDS_HandActionData.Cast(action_data);
		if (!data)
		{
			return true;
		}

		if (data.m_Spent != 0)
		{
			return false;
		}

		return true;
	}
};

modded class ActionBandageSelf
{
	override void CreateConditionComponents()
	{
		super.CreateConditionComponents();
		GetVariantManager().GetOnUpdateInvoker().Clear();
		GetVariantManager().GetOnUpdateInvoker().Insert(OnOfferVariants);
	}

	override ActionData CreateActionData()
	{
		return new OMD_IDS_HandActionData();
	}

	void OnOfferVariants(Object item, Object target, int componentIndex)
	{
		int count;

		count = OMD_IDS_HandSnapshot.BandageCount();
		if (count < 1)
		{
			count = 1;
		}
		else
		{
			count = OMD_IDS_HandSnapshot.BandageOfferCount();
			if (count < 1)
			{
				count = 1;
			}
		}

		GetVariantManager().SetActionVariantCount(count);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		string named;
		int zoneId;

		if (OMD_IDS_HandSnapshot.BandageCount() < 1)
		{
			m_Text = "#treat_wound";
			return;
		}

		named = OMD_IDS_HandSnapshot.NamedBandage(GetVariantID());
		if (named == "")
		{
			zoneId = OMD_IDS_HandSnapshot.OfferZone(GetVariantID(), 2);
			m_Text = OMD_IDS_LimbCatalog.PairOrZone(GetVariantID(), zoneId, "Забинтовать кисть", "Забинтовать другую кисть", "Забинтовать");
			return;
		}

		m_Text = named;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		int count;
		int variantId;
		int zoneId;
		OMD_IDS_TreatmentHooks hooks;

		if (!player)
		{
			return false;
		}

		if (!player.IsAlive())
		{
			return false;
		}

		if (!GetGame())
		{
			return false;
		}

		if (GetGame().IsServer())
		{
			hooks = OMD_IDS_TreatmentHooks.Get();
			if (!hooks.BandageActive())
			{
				return super.ActionCondition(player, target, item);
			}

			variantId = OMD_IDS_HandSnapshot.VariantOf(this, player);
			zoneId = hooks.BandageZone(player, variantId);
			if (zoneId == 0)
			{
				return false;
			}

			return hooks.CanBandageServer(player, zoneId);
		}

		count = OMD_IDS_HandSnapshot.BandageCount();
		if (count < 1)
		{
			return player.IsBleeding();
		}

		count = OMD_IDS_HandSnapshot.BandageOfferCount();
		if (count < 1)
		{
			return false;
		}

		variantId = GetVariantID();
		if (variantId < 0)
		{
			return false;
		}

		if (variantId >= count)
		{
			return false;
		}

		return true;
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		if (!OMD_IDS_BandageWork.Continue(action_data))
		{
			return false;
		}

		return super.ActionConditionContinue(action_data);
	}

	override bool ReadFromContext(ParamsReadContext ctx, out ActionReciveData action_recive_data)
	{
		if (!action_recive_data)
		{
			action_recive_data = new OMD_IDS_HandActionReciveData();
		}

		if (!super.ReadFromContext(ctx, action_recive_data))
		{
			return false;
		}

		return OMD_IDS_SutureWork.ReadVariant(ctx, action_recive_data);
	}

	override void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
	{
		super.WriteToContext(ctx, action_data);
		OMD_IDS_SutureWork.WriteVariant(ctx, action_data, this);
	}

	override void HandleReciveData(ActionReciveData action_recive_data, ActionData action_data)
	{
		super.HandleReciveData(action_recive_data, action_data);
		OMD_IDS_SutureWork.KeepVariant(action_recive_data, action_data);
	}

	override bool Post_SetupAction(ActionData action_data)
	{
		OMD_IDS_HandActionData data;

		if (!GetGame())
		{
			return super.Post_SetupAction(action_data);
		}

		if (!GetGame().IsDedicatedServer())
		{
			data = OMD_IDS_HandActionData.Cast(action_data);
			if (data)
			{
				data.m_HandVariant = GetVariantID();
			}

			if (action_data)
			{
				if (action_data.m_Player)
				{
					action_data.m_Player.OMD_IDS_SetHandVariant(GetVariantID());
				}
			}
		}

		return super.Post_SetupAction(action_data);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		OMD_IDS_TreatmentHooks hooks;

		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		hooks = OMD_IDS_TreatmentHooks.Get();
		if (!hooks.BandageActive())
		{
			super.OnFinishProgressServer(action_data);
			return;
		}

		OMD_IDS_BandageWork.Finish(action_data, 1);
	}
};

modded class ActionBandageTarget
{
	override void CreateConditionComponents()
	{
		super.CreateConditionComponents();
		GetVariantManager().GetOnUpdateInvoker().Clear();
		GetVariantManager().GetOnUpdateInvoker().Insert(OnOfferVariants);
	}

	override ActionData CreateActionData()
	{
		return new OMD_IDS_HandActionData();
	}

	void OnOfferVariants(Object item, Object target, int componentIndex)
	{
		int count;
		PlayerBase patient;

		count = 0;
		patient = null;
		if (Class.CastTo(patient, target))
		{
			count = OMD_IDS_HandSnapshot.OfferCountOf(patient, 2);
		}

		GetVariantManager().SetActionVariantCount(count);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		string named;
		PlayerBase patient;

		patient = OMD_IDS_HandSnapshot.OtherPlayer(target);
		if (!patient)
		{
			m_Text = "#treat_persons_wound";
			return;
		}

		if (!patient.IsAlive())
		{
			m_Text = "#treat_persons_wound";
			return;
		}

		named = OMD_IDS_HandSnapshot.NamedBandageOf(patient, GetVariantID());
		if (named == "")
		{
			m_Text = "#treat_persons_wound";
			return;
		}

		m_Text = named;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		int variantId;
		int zoneId;
		PlayerBase patient;
		OMD_IDS_TreatmentHooks hooks;

		patient = OMD_IDS_HandSnapshot.OtherPlayer(target);
		if (!patient)
		{
			return false;
		}

		if (!patient.IsAlive())
		{
			return false;
		}

		if (!GetGame())
		{
			return false;
		}

		if (GetGame().IsServer())
		{
			hooks = OMD_IDS_TreatmentHooks.Get();
			if (!hooks.BandageActive())
			{
				return super.ActionCondition(player, target, item);
			}

			variantId = OMD_IDS_HandSnapshot.VariantOf(this, player);
			zoneId = hooks.BandageZone(patient, variantId);
			if (zoneId == 0)
			{
				return false;
			}

			return hooks.CanBandageServer(patient, zoneId);
		}

		if (OMD_IDS_HandSnapshot.VariantOkOf(patient, GetVariantID(), 2) == 0)
		{
			return false;
		}

		return true;
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		if (!OMD_IDS_BandageWork.Continue(action_data))
		{
			return false;
		}

		return super.ActionConditionContinue(action_data);
	}

	override bool CanTargetBeInVehicle()
	{
		return true;
	}

	override bool ReadFromContext(ParamsReadContext ctx, out ActionReciveData action_recive_data)
	{
		if (!action_recive_data)
		{
			action_recive_data = new OMD_IDS_HandActionReciveData();
		}

		if (!super.ReadFromContext(ctx, action_recive_data))
		{
			return false;
		}

		return OMD_IDS_SutureWork.ReadVariant(ctx, action_recive_data);
	}

	override void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
	{
		super.WriteToContext(ctx, action_data);
		OMD_IDS_SutureWork.WriteVariant(ctx, action_data, this);
	}

	override void HandleReciveData(ActionReciveData action_recive_data, ActionData action_data)
	{
		super.HandleReciveData(action_recive_data, action_data);
		OMD_IDS_SutureWork.KeepVariant(action_recive_data, action_data);
	}

	override bool Post_SetupAction(ActionData action_data)
	{
		OMD_IDS_HandActionData data;

		if (!GetGame())
		{
			return super.Post_SetupAction(action_data);
		}

		if (!GetGame().IsDedicatedServer())
		{
			data = OMD_IDS_HandActionData.Cast(action_data);
			if (data)
			{
				data.m_HandVariant = GetVariantID();
			}

			if (action_data)
			{
				if (action_data.m_Player)
				{
					action_data.m_Player.OMD_IDS_SetHandVariant(GetVariantID());
				}
			}
		}

		return super.Post_SetupAction(action_data);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		OMD_IDS_TreatmentHooks hooks;

		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		hooks = OMD_IDS_TreatmentHooks.Get();
		if (!hooks.BandageActive())
		{
			super.OnFinishProgressServer(action_data);
			return;
		}

		OMD_IDS_BandageWork.Finish(action_data, 0);
	}

	override void OnFinishProgressClient(ActionData action_data)
	{
		super.OnFinishProgressClient(action_data);
	}
};

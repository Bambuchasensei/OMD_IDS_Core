// Legs with BROKEN_LEGS stay on the vanilla path (super).
// A hand ClosedFracture or OpenFracture uses the server hook and does not call ApplySplint.
class OMD_IDS_SplintWork
{
	static void FinishHand(ActionData action_data, int selfUse)
	{
		int zoneId;
		int variantId;
		PlayerBase actor;
		PlayerBase patient;
		ItemBase item;
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

		if (!patient)
		{
			return;
		}

		if (!patient.IsAlive())
		{
			return;
		}

		if (patient.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
		{
			return;
		}

		variantId = 0;
		data = OMD_IDS_HandActionData.Cast(action_data);
		if (data)
		{
			variantId = data.m_HandVariant;
		}

		hooks = OMD_IDS_TreatmentHooks.Get();
		zoneId = hooks.SplintZone(patient, variantId);
		if (zoneId == 0)
		{
			hooks.OnTreatRefused(actor, patient, zoneId, OMD_IDS_TreatmentHooks.OP_SPLINT);
			return;
		}

		if (!hooks.CanSplintServer(patient, zoneId))
		{
			hooks.OnTreatRefused(actor, patient, zoneId, OMD_IDS_TreatmentHooks.OP_SPLINT);
			return;
		}

		hooks.OnSplint(actor, patient, zoneId);
		item = action_data.m_MainItem;
		if (!item)
		{
			return;
		}

		item.Delete();
	}

	static int OfferCount(ActionBase action, Object target)
	{
		int count;
		PlayerBase actor;
		PlayerBase patient;
		Man man;

		count = 0;
		if (!GetGame())
		{
			return count;
		}

		man = GetGame().GetPlayer();
		if (!Class.CastTo(actor, man))
		{
			return count;
		}

		patient = actor;
		if (action)
		{
			if (action.HasTarget())
			{
				patient = null;
				if (!Class.CastTo(patient, target))
				{
					return 0;
				}
			}
		}

		if (!patient)
		{
			return 0;
		}

		if (patient.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
		{
			return 1;
		}

		if (action)
		{
			if (action.HasTarget())
			{
				if (!patient.IsAlive())
				{
					return 0;
				}

				return OMD_IDS_HandSnapshot.OfferCountOf(patient, 1);
			}
		}

		return OMD_IDS_HandSnapshot.SplintOfferCount();
	}
};

modded class ActionSplintSelf
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

		count = OMD_IDS_SplintWork.OfferCount(this, target);
		GetVariantManager().SetActionVariantCount(count);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		string named;

		if (player)
		{
			if (player.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			{
				m_Text = "#apply_splint";
				return;
			}
		}

		named = OMD_IDS_HandSnapshot.NamedSplint(GetVariantID());
		if (named == "")
		{
			m_Text = "Наложить шину";
			if (GetVariantID() == 1)
			{
				m_Text = "Шина на другую кисть";
			}

			return;
		}

		m_Text = named;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		int variantId;
		int zoneId;
		OMD_IDS_TreatmentHooks hooks;

		if (!player)
		{
			return false;
		}

		if (player.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
		{
			return super.ActionCondition(player, target, item);
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
			variantId = OMD_IDS_HandSnapshot.VariantOf(this, player);
			hooks = OMD_IDS_TreatmentHooks.Get();
			zoneId = hooks.SplintZone(player, variantId);
			if (zoneId == 0)
			{
				return false;
			}

			return hooks.CanSplintServer(player, zoneId);
		}

		variantId = GetVariantID();
		if (OMD_IDS_HandSnapshot.SplintOfferCount() < 1)
		{
			return false;
		}

		if (variantId == 0)
		{
			return true;
		}

		if (variantId == 1)
		{
			if (OMD_IDS_HandSnapshot.SplintOfferCount() >= 2)
			{
				return true;
			}
		}

		return false;
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
		PlayerBase patient;

		patient = null;
		if (action_data)
		{
			patient = action_data.m_Player;
		}

		if (patient)
		{
			if (patient.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			{
				super.OnFinishProgressServer(action_data);
				return;
			}
		}

		OMD_IDS_SplintWork.FinishHand(action_data, 1);
	}
};

modded class ActionSplintTarget
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

		count = OMD_IDS_SplintWork.OfferCount(this, target);
		GetVariantManager().SetActionVariantCount(count);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		string named;
		PlayerBase patient;

		patient = OMD_IDS_HandSnapshot.OtherPlayer(target);
		if (patient)
		{
			if (patient.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			{
				m_Text = "#fix_persons_fracture";
				return;
			}
		}

		named = OMD_IDS_HandSnapshot.NamedSplintOf(patient, GetVariantID());
		if (named == "")
		{
			m_Text = "Наложить шину";
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

		if (patient.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
		{
			return super.ActionCondition(player, target, item);
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
			variantId = OMD_IDS_HandSnapshot.VariantOf(this, player);
			hooks = OMD_IDS_TreatmentHooks.Get();
			zoneId = hooks.SplintZone(patient, variantId);
			if (zoneId == 0)
			{
				return false;
			}

			return hooks.CanSplintServer(patient, zoneId);
		}

		if (OMD_IDS_HandSnapshot.VariantOkOf(patient, GetVariantID(), 1) == 0)
		{
			return false;
		}

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
		PlayerBase patient;

		patient = null;
		if (action_data)
		{
			patient = OMD_IDS_HandSnapshot.OtherPlayer(action_data.m_Target);
		}

		if (patient)
		{
			if (patient.GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			{
				super.OnFinishProgressServer(action_data);
				return;
			}
		}

		OMD_IDS_SplintWork.FinishHand(action_data, 0);
	}
};

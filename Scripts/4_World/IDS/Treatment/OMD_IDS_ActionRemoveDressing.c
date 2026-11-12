// Remove a hand dressing with empty hands. The bandage item is not returned.
// Time is heal.removeDressingDurationSec, factory 3. Self and other use the vanilla bandage commands.
// Self: P:\scripts\4_world\classes\useractionscomponent\actions\continuous\medical\actionbandageself.c:20-22
// P:\scripts\3_game\dayzplayer.c:827 CMD_ACTIONFB_BANDAGE = 58, full body, crouch only.
// Other: P:\scripts\4_world\classes\useractionscomponent\actions\continuous\medical\actionbandagetarget.c:20-22
// P:\scripts\3_game\dayzplayer.c:831 CMD_ACTIONFB_BANDAGETARGET = 63, full body, erect or crouch.
// Empty hands: CCINotPresent, P:\scripts\4_world\classes\useractionscomponent\itemconditioncomponents\ccinotpresent.c:5
// Time: void CAContinuousTime(float time_to_complete_action), P:\scripts\4_world\classes\useractionscomponent\actioncomponents\cacontinuoustime.c:9
// Hand choice is PickHand mode 3. Variant 0 / 1 uses the same action channel as the suture.
class OMD_IDS_DressingRemoveWork
{
	static void Finish(ActionData action_data, int selfUse)
	{
		int zoneId;
		int variantId;
		PlayerBase actor;
		PlayerBase patient;
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

		variantId = 0;
		data = OMD_IDS_HandActionData.Cast(action_data);
		if (data)
		{
			variantId = data.m_HandVariant;
		}

		hooks = OMD_IDS_TreatmentHooks.Get();
		zoneId = hooks.RemoveDressingZone(patient, variantId);
		if (zoneId == 0)
		{
			hooks.OnTreatRefused(actor, patient, zoneId, OMD_IDS_TreatmentHooks.OP_REMOVE);
			return;
		}

		if (!hooks.CanRemoveDressingServer(patient, zoneId))
		{
			hooks.OnTreatRefused(actor, patient, zoneId, OMD_IDS_TreatmentHooks.OP_REMOVE);
			return;
		}

		hooks.OnRemoveDressing(actor, patient, zoneId);
	}
};

class OMD_IDS_ActionRemoveDressingSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float seconds;

		seconds = OMD_IDS_TreatmentHooks.Get().RemoveDressingDurationSec();
		m_ActionData.m_ActionComponent = new CAContinuousTime(seconds);
	}
};

class OMD_IDS_ActionRemoveDressingSelf : ActionContinuousBase
{
	void OMD_IDS_ActionRemoveDressingSelf()
	{
		m_CallbackClass = OMD_IDS_ActionRemoveDressingSelfCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Снять повязку";
		GetVariantManager().GetOnUpdateInvoker().Insert(OnOfferVariants);
	}

	override ActionData CreateActionData()
	{
		return new OMD_IDS_HandActionData();
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINotPresent();
		m_ConditionTarget = new CCTSelf();
	}

	override bool HasTarget()
	{
		return false;
	}

	void OnOfferVariants(Object item, Object target, int componentIndex)
	{
		int count;

		count = OMD_IDS_HandSnapshot.RemoveDressingOfferCount();
		GetVariantManager().SetActionVariantCount(count);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		int zoneId;

		zoneId = OMD_IDS_HandSnapshot.OfferZone(GetVariantID(), 3);
		m_Text = OMD_IDS_LimbCatalog.PairOrZone(GetVariantID(), zoneId, "Снять повязку", "Снять повязку с другой кисти", "Снять повязку с");
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

		if (item)
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
			zoneId = hooks.RemoveDressingZone(player, variantId);
			if (zoneId == 0)
			{
				return false;
			}

			return hooks.CanRemoveDressingServer(player, zoneId);
		}

		count = OMD_IDS_HandSnapshot.RemoveDressingOfferCount();
		variantId = GetVariantID();
		if (count < 1)
		{
			return false;
		}

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
		OMD_IDS_DressingRemoveWork.Finish(action_data, 1);
	}
};

class OMD_IDS_ActionRemoveDressingTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float seconds;

		seconds = OMD_IDS_TreatmentHooks.Get().RemoveDressingDurationSec();
		m_ActionData.m_ActionComponent = new CAContinuousTime(seconds);
	}
};

class OMD_IDS_ActionRemoveDressingTarget : ActionContinuousBase
{
	void OMD_IDS_ActionRemoveDressingTarget()
	{
		m_CallbackClass = OMD_IDS_ActionRemoveDressingTargetCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Снять повязку";
		GetVariantManager().GetOnUpdateInvoker().Insert(OnOfferVariants);
	}

	override ActionData CreateActionData()
	{
		return new OMD_IDS_HandActionData();
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINotPresent();
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override bool CanTargetBeInVehicle()
	{
		return true;
	}

	// Target list is filled only when the input has m_DetectFromTarget.
	// ContinuousInteractActionInput sets it true: P:\scripts\4_world\classes\useractionscomponent\actioninput.c:552
	// ContinuousDefaultActionInput leaves it false: same file, line 635.
	// ActionCheckPulseTarget uses this input: P:\scripts\4_world\classes\useractionscomponent\actions\continuous\medical\actioncheckpulsetarget.c:78
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	void OnOfferVariants(Object item, Object target, int componentIndex)
	{
		int count;
		PlayerBase patient;

		count = 0;
		patient = null;
		if (Class.CastTo(patient, target))
		{
			count = OMD_IDS_HandSnapshot.OfferCountOf(patient, 3);
		}

		GetVariantManager().SetActionVariantCount(count);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		string named;
		PlayerBase patient;

		patient = OMD_IDS_HandSnapshot.OtherPlayer(target);
		named = OMD_IDS_HandSnapshot.NamedRemoveOf(patient, GetVariantID());
		if (named == "")
		{
			m_Text = "Снять повязку";
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

		if (item)
		{
			return false;
		}

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
			variantId = OMD_IDS_HandSnapshot.VariantOf(this, player);
			hooks = OMD_IDS_TreatmentHooks.Get();
			zoneId = hooks.RemoveDressingZone(patient, variantId);
			if (zoneId == 0)
			{
				return false;
			}

			return hooks.CanRemoveDressingServer(patient, zoneId);
		}

		if (OMD_IDS_HandSnapshot.VariantOkOf(patient, GetVariantID(), 3) == 0)
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
		OMD_IDS_DressingRemoveWork.Finish(action_data, 0);
	}
};

modded class PlayerBase
{
	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);
		AddAction(OMD_IDS_ActionRemoveDressingSelf, InputActionMap);
	}

	override void SetActionsRemoteTarget(out TInputActionMap InputActionMap)
	{
		super.SetActionsRemoteTarget(InputActionMap);
		AddAction(OMD_IDS_ActionRemoveDressingTarget, InputActionMap);
	}
};

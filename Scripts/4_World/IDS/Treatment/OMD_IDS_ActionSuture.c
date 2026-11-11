// Hand suture. Vanilla ActionSewSelf and ActionSewTarget are not modded.
// Time is heal.sutureDurationSec, factory 10. Self animation 71, other animation 60.
// P:\scripts\3_game\dayzplayer.c:839 CMD_ACTIONFB_STITCHUPSELF = 71
// P:\scripts\3_game\dayzplayer.c:829 CMD_ACTIONFB_INTERACT = 60
// Quantity 20 and chances 30 / 40 match P:\InediaPain\config.cpp:607-612.
class OMD_IDS_SutureWork
{
	// Read when the bar is created. Pain scales it once in Setup.
	static float NominalSec()
	{
		return OMD_IDS_TreatmentHooks.Get().SutureDurationSec();
	}

	static void Finish(ActionData action_data, int selfUse)
	{
		int zoneId;
		int variantId;
		int chance;
		int roll;
		float rawChance;
		bool success;
		PlayerBase actor;
		PlayerBase patient;
		ItemBase item;
		OMD_IDS_HandActionData data;
		OMD_IDS_TreatmentHooks hooks;
		PluginTransmissionAgents agents;

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
		zoneId = hooks.SutureZone(patient, variantId);
		if (zoneId == 0)
		{
			hooks.OnTreatRefused(actor, patient, zoneId, OMD_IDS_TreatmentHooks.OP_SUTURE);
			return;
		}

		if (!hooks.CanSutureServer(patient, zoneId))
		{
			hooks.OnTreatRefused(actor, patient, zoneId, OMD_IDS_TreatmentHooks.OP_SUTURE);
			return;
		}

		rawChance = hooks.SutureChanceOther();
		if (selfUse != 0)
		{
			rawChance = hooks.SutureChanceSelf();
		}

		chance = rawChance;
		if (chance < 0)
		{
			chance = 0;
		}

		if (chance > 100)
		{
			chance = 100;
		}

		roll = Math.RandomIntInclusive(1, 100);
		success = false;
		if (roll <= chance)
		{
			success = true;
		}

		hooks.OnSuture(actor, patient, zoneId, success);
		item = action_data.m_MainItem;
		if (!item)
		{
			return;
		}

		if (Class.CastTo(agents, GetPlugin(PluginTransmissionAgents)))
		{
			agents.TransmitAgents(item, patient, AGT_ITEM_TO_FLESH);
		}

		if (item.HasQuantity())
		{
			item.AddQuantity(-20, true);
			return;
		}

		item.Delete();
	}

	static void WriteVariant(ParamsWriteContext ctx, ActionData action_data, ActionBase action)
	{
		int variantId;

		variantId = 0;
		if (action)
		{
			variantId = action.GetVariantID();
		}

		ctx.Write(variantId);
	}

	static bool ReadVariant(ParamsReadContext ctx, ActionReciveData action_recive_data)
	{
		int variantId;
		OMD_IDS_HandActionReciveData received;

		variantId = 0;
		if (!ctx.Read(variantId))
		{
			return false;
		}

		received = OMD_IDS_HandActionReciveData.Cast(action_recive_data);
		if (!received)
		{
			return false;
		}

		received.m_HandVariant = variantId;
		return true;
	}

	static void KeepVariant(ActionReciveData action_recive_data, ActionData action_data)
	{
		int variantId;
		OMD_IDS_HandActionReciveData received;
		OMD_IDS_HandActionData data;

		variantId = 0;
		received = OMD_IDS_HandActionReciveData.Cast(action_recive_data);
		if (received)
		{
			variantId = received.m_HandVariant;
		}

		data = OMD_IDS_HandActionData.Cast(action_data);
		if (data)
		{
			data.m_HandVariant = variantId;
		}

		if (!action_data)
		{
			return;
		}

		if (!action_data.m_Player)
		{
			return;
		}

		action_data.m_Player.OMD_IDS_SetHandVariant(variantId);
	}

	static PlayerBase PatientOf(ActionData action_data, int selfUse)
	{
		PlayerBase patient;

		patient = null;
		if (!action_data)
		{
			return patient;
		}

		patient = action_data.m_Player;
		if (selfUse == 0)
		{
			patient = OMD_IDS_HandSnapshot.OtherPlayer(action_data.m_Target);
		}

		return patient;
	}

	// Duration matches CAContinuousTime.Setup: NominalSec, then one pain multiplier.
	static void BeginShock(ActionData action_data, int selfUse)
	{
		int zoneId;
		int variantId;
		float extra;
		float duration;
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

		data = OMD_IDS_HandActionData.Cast(action_data);
		if (!data)
		{
			return;
		}

		data.m_SutureOn = 0;
		data.m_SutureZone = 0;
		data.m_SutureStartMs = 0;
		data.m_SutureDue = 0;
		data.m_SutureDuration = 0;
		patient = PatientOf(action_data, selfUse);
		if (!patient)
		{
			return;
		}

		variantId = data.m_HandVariant;
		hooks = OMD_IDS_TreatmentHooks.Get();
		zoneId = hooks.SutureZone(patient, variantId);
		if (zoneId == 0)
		{
			return;
		}

		extra = OMD_IDS_PainActions.Extra(action_data);
		duration = NominalSec();
		if (extra > 0)
		{
			duration = duration * (1 + extra);
		}

		if (duration <= 0)
		{
			return;
		}

		data.m_SutureOn = 1;
		data.m_SutureZone = zoneId;
		data.m_SutureStartMs = GetGame().GetTime();
		data.m_SutureDue = 0;
		data.m_SutureDuration = duration;
		hooks.BeginSutureShock(patient, zoneId);
	}

	static void TickShock(ActionData action_data, int selfUse, int forceFull)
	{
		int nowMs;
		int elapsedMs;
		int dueCount;
		int maxCount;
		int index;
		float elapsedSec;
		float tickSec;
		float total;
		float duration;
		float portion;
		float dueF;
		float maxF;
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

		data = OMD_IDS_HandActionData.Cast(action_data);
		if (!data)
		{
			return;
		}

		if (data.m_SutureOn == 0)
		{
			return;
		}

		patient = PatientOf(action_data, selfUse);
		if (!patient)
		{
			return;
		}

		hooks = OMD_IDS_TreatmentHooks.Get();
		tickSec = hooks.SutureShockTickSec();
		total = hooks.SutureShockTotal();
		duration = data.m_SutureDuration;
		if (tickSec <= 0)
		{
			return;
		}

		if (total <= 0)
		{
			return;
		}

		if (duration <= 0)
		{
			return;
		}

		nowMs = GetGame().GetTime();
		elapsedMs = nowMs - data.m_SutureStartMs;
		if (elapsedMs < 0)
		{
			elapsedMs = 0;
		}

		elapsedSec = elapsedMs;
		elapsedSec = elapsedSec / 1000.0;
		dueF = elapsedSec / tickSec;
		dueF = dueF + 0.001;
		dueCount = dueF;
		maxF = duration / tickSec;
		maxF = maxF + 0.001;
		maxCount = maxF;
		if (dueCount > maxCount)
		{
			dueCount = maxCount;
		}

		if (dueCount < 0)
		{
			dueCount = 0;
		}

		if (forceFull != 0)
		{
			dueCount = maxCount;
		}

		if (dueCount <= data.m_SutureDue)
		{
			return;
		}

		portion = total * tickSec / duration;
		for (index = data.m_SutureDue; index < dueCount; index = index + 1)
		{
			hooks.ApplySutureShock(patient, data.m_SutureZone, portion);
		}

		data.m_SutureDue = dueCount;
	}

	static void EndShock(ActionData action_data, int selfUse)
	{
		int zoneId;
		int forceFull;
		PlayerBase patient;
		OMD_IDS_HandActionData data;
		OMD_IDS_TreatmentHooks hooks;

		forceFull = 0;
		if (action_data)
		{
			if (action_data.m_State == UA_FINISHED)
			{
				forceFull = 1;
			}
		}

		TickShock(action_data, selfUse, forceFull);
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

		data = OMD_IDS_HandActionData.Cast(action_data);
		if (!data)
		{
			return;
		}

		if (data.m_SutureOn == 0)
		{
			return;
		}

		zoneId = data.m_SutureZone;
		data.m_SutureOn = 0;
		patient = PatientOf(action_data, selfUse);
		if (!patient)
		{
			return;
		}

		hooks = OMD_IDS_TreatmentHooks.Get();
		hooks.EndSutureShock(patient, zoneId);
	}
};

class OMD_IDS_ActionSutureSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float seconds;

		seconds = OMD_IDS_SutureWork.NominalSec();
		m_ActionData.m_ActionComponent = new CAContinuousTime(seconds);
	}
};

class OMD_IDS_ActionSutureSelf : ActionContinuousBase
{
	void OMD_IDS_ActionSutureSelf()
	{
		m_CallbackClass = OMD_IDS_ActionSutureSelfCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STITCHUPSELF;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Зашить кисть";
		GetVariantManager().GetOnUpdateInvoker().Insert(OnOfferVariants);
	}

	override ActionData CreateActionData()
	{
		return new OMD_IDS_HandActionData();
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTSelf();
	}

	override bool HasTarget()
	{
		return false;
	}

	void OnOfferVariants(Object item, Object target, int componentIndex)
	{
		int count;

		count = OMD_IDS_HandSnapshot.SutureOfferCount();
		GetVariantManager().SetActionVariantCount(count);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		string named;

		named = OMD_IDS_HandSnapshot.NamedSuture(GetVariantID());
		if (named == "")
		{
			m_Text = "Зашить кисть";
			if (GetVariantID() == 1)
			{
				m_Text = "Зашить другую кисть";
			}

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
			variantId = OMD_IDS_HandSnapshot.VariantOf(this, player);
			hooks = OMD_IDS_TreatmentHooks.Get();
			zoneId = hooks.SutureZone(player, variantId);
			if (zoneId == 0)
			{
				return false;
			}

			return hooks.CanSutureServer(player, zoneId);
		}

		count = OMD_IDS_HandSnapshot.SutureOfferCount();
		variantId = GetVariantID();
		if (count < 1)
		{
			return false;
		}

		if (variantId == 0)
		{
			return true;
		}

		if (variantId == 1)
		{
			if (count >= 2)
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

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		OMD_IDS_SutureWork.BeginShock(action_data, 1);
	}

	override void OnUpdateServer(ActionData action_data)
	{
		super.OnUpdateServer(action_data);
		OMD_IDS_SutureWork.TickShock(action_data, 1, 0);
	}

	override void OnEndServer(ActionData action_data)
	{
		OMD_IDS_SutureWork.EndShock(action_data, 1);
		super.OnEndServer(action_data);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		OMD_IDS_SutureWork.Finish(action_data, 1);
	}
};

class OMD_IDS_ActionSutureTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float seconds;

		seconds = OMD_IDS_SutureWork.NominalSec();
		m_ActionData.m_ActionComponent = new CAContinuousTime(seconds);
	}
};

class OMD_IDS_ActionSutureTarget : ActionContinuousBase
{
	void OMD_IDS_ActionSutureTarget()
	{
		m_CallbackClass = OMD_IDS_ActionSutureTargetCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Зашить кисть";
		GetVariantManager().GetOnUpdateInvoker().Insert(OnOfferVariants);
	}

	override ActionData CreateActionData()
	{
		return new OMD_IDS_HandActionData();
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
	}

	override bool CanTargetBeInVehicle()
	{
		return true;
	}

	void OnOfferVariants(Object item, Object target, int componentIndex)
	{
		int count;
		PlayerBase patient;

		count = 0;
		patient = null;
		if (Class.CastTo(patient, target))
		{
			count = OMD_IDS_HandSnapshot.OfferCountOf(patient, 0);
		}

		GetVariantManager().SetActionVariantCount(count);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		string named;
		PlayerBase patient;

		patient = OMD_IDS_HandSnapshot.OtherPlayer(target);
		named = OMD_IDS_HandSnapshot.NamedSutureOf(patient, GetVariantID());
		if (named == "")
		{
			m_Text = "Зашить кисть";
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
			variantId = OMD_IDS_HandSnapshot.VariantOf(this, player);
			hooks = OMD_IDS_TreatmentHooks.Get();
			zoneId = hooks.SutureZone(patient, variantId);
			if (zoneId == 0)
			{
				return false;
			}

			return hooks.CanSutureServer(patient, zoneId);
		}

		if (OMD_IDS_HandSnapshot.VariantOkOf(patient, GetVariantID(), 0) == 0)
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

	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		OMD_IDS_SutureWork.BeginShock(action_data, 0);
	}

	override void OnUpdateServer(ActionData action_data)
	{
		super.OnUpdateServer(action_data);
		OMD_IDS_SutureWork.TickShock(action_data, 0, 0);
	}

	override void OnEndServer(ActionData action_data)
	{
		OMD_IDS_SutureWork.EndShock(action_data, 0);
		super.OnEndServer(action_data);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		OMD_IDS_SutureWork.Finish(action_data, 0);
	}
};

modded class SewingKit
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(OMD_IDS_ActionSutureSelf);
		AddAction(OMD_IDS_ActionSutureTarget);
	}
};

// Pull one stuck bullet out of a hand with a HuntingKnife.
// Same animations as the suture: self 71, other 60.
// P:\scripts\3_game\dayzplayer.c:839 CMD_ACTIONFB_STITCHUPSELF = 71
// P:\scripts\3_game\dayzplayer.c:829 CMD_ACTIONFB_INTERACT = 60
// Duration, shock and the knife numbers are read through OMD_IDS_TreatmentHooks.
// The client stubs return the factory values. The server override reads injuries.json.
class OMD_IDS_ExtractWork
{
	static void Finish(ActionData action_data, int selfUse)
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
		zoneId = hooks.ExtractZone(patient, variantId);
		if (zoneId == 0)
		{
			hooks.OnTreatRefused(actor, patient, zoneId, OMD_IDS_TreatmentHooks.OP_EXTRACT);
			return;
		}

		if (!hooks.CanExtractServer(patient, zoneId))
		{
			hooks.OnTreatRefused(actor, patient, zoneId, OMD_IDS_TreatmentHooks.OP_EXTRACT);
			return;
		}

		item = action_data.m_MainItem;
		hooks.OnExtract(actor, patient, zoneId, item);
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

	// Duration is the hook value, then the same pain multiplier CAContinuousTime.Setup applies once.
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

		data.m_ExtractOn = 0;
		data.m_ExtractZone = 0;
		data.m_ExtractStartMs = 0;
		data.m_ExtractDue = 0;
		data.m_ExtractDuration = 0;
		patient = PatientOf(action_data, selfUse);
		if (!patient)
		{
			return;
		}

		variantId = data.m_HandVariant;
		hooks = OMD_IDS_TreatmentHooks.Get();
		zoneId = hooks.ExtractZone(patient, variantId);
		if (zoneId == 0)
		{
			return;
		}

		extra = OMD_IDS_PainActions.Extra(action_data);
		duration = hooks.ExtractDurationSec();
		if (extra > 0)
		{
			duration = duration * (1 + extra);
		}

		if (duration <= 0)
		{
			return;
		}

		data.m_ExtractOn = 1;
		data.m_ExtractZone = zoneId;
		data.m_ExtractStartMs = GetGame().GetTime();
		data.m_ExtractDue = 0;
		data.m_ExtractDuration = duration;
		hooks.BeginExtractShock(patient, zoneId);
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

		if (data.m_ExtractOn == 0)
		{
			return;
		}

		patient = PatientOf(action_data, selfUse);
		if (!patient)
		{
			return;
		}

		hooks = OMD_IDS_TreatmentHooks.Get();
		tickSec = hooks.ExtractShockTickSec();
		total = hooks.ExtractShockTotal();
		duration = data.m_ExtractDuration;
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
		elapsedMs = nowMs - data.m_ExtractStartMs;
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

		if (dueCount <= data.m_ExtractDue)
		{
			return;
		}

		portion = total * tickSec / duration;
		for (index = data.m_ExtractDue; index < dueCount; index = index + 1)
		{
			hooks.ApplyExtractShock(patient, data.m_ExtractZone, portion);
		}

		data.m_ExtractDue = dueCount;
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

		if (data.m_ExtractOn == 0)
		{
			return;
		}

		zoneId = data.m_ExtractZone;
		data.m_ExtractOn = 0;
		patient = PatientOf(action_data, selfUse);
		if (!patient)
		{
			return;
		}

		hooks = OMD_IDS_TreatmentHooks.Get();
		hooks.EndExtractShock(patient, zoneId);
	}
};

class OMD_IDS_ActionExtractBulletSelfCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float seconds;

		seconds = OMD_IDS_TreatmentHooks.Get().ExtractDurationSec();
		m_ActionData.m_ActionComponent = new CAContinuousTime(seconds);
	}
};

class OMD_IDS_ActionExtractBulletSelf : ActionContinuousBase
{
	void OMD_IDS_ActionExtractBulletSelf()
	{
		m_CallbackClass = OMD_IDS_ActionExtractBulletSelfCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_STITCHUPSELF;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Вытащить пулю";
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

		count = OMD_IDS_HandSnapshot.ExtractOfferCount();
		GetVariantManager().SetActionVariantCount(count);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		string named;

		named = OMD_IDS_HandSnapshot.NamedExtract(GetVariantID());
		if (named == "")
		{
			m_Text = "Вытащить пулю";
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
			zoneId = hooks.ExtractZone(player, variantId);
			if (zoneId == 0)
			{
				return false;
			}

			return hooks.CanExtractServer(player, zoneId);
		}

		count = OMD_IDS_HandSnapshot.ExtractOfferCount();
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
		OMD_IDS_ExtractWork.BeginShock(action_data, 1);
	}

	override void OnUpdateServer(ActionData action_data)
	{
		super.OnUpdateServer(action_data);
		OMD_IDS_ExtractWork.TickShock(action_data, 1, 0);
	}

	override void OnEndServer(ActionData action_data)
	{
		OMD_IDS_ExtractWork.EndShock(action_data, 1);
		super.OnEndServer(action_data);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		OMD_IDS_ExtractWork.Finish(action_data, 1);
	}
};

class OMD_IDS_ActionExtractBulletTargetCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float seconds;

		seconds = OMD_IDS_TreatmentHooks.Get().ExtractDurationSec();
		m_ActionData.m_ActionComponent = new CAContinuousTime(seconds);
	}
};

class OMD_IDS_ActionExtractBulletTarget : ActionContinuousBase
{
	void OMD_IDS_ActionExtractBulletTarget()
	{
		m_CallbackClass = OMD_IDS_ActionExtractBulletTargetCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Вытащить пулю";
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
			count = OMD_IDS_HandSnapshot.OfferCountOf(patient, 4);
		}

		GetVariantManager().SetActionVariantCount(count);
	}

	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		string named;
		PlayerBase patient;

		patient = OMD_IDS_HandSnapshot.OtherPlayer(target);
		named = OMD_IDS_HandSnapshot.NamedExtractOf(patient, GetVariantID());
		if (named == "")
		{
			m_Text = "Вытащить пулю";
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
			zoneId = hooks.ExtractZone(patient, variantId);
			if (zoneId == 0)
			{
				return false;
			}

			return hooks.CanExtractServer(patient, zoneId);
		}

		if (OMD_IDS_HandSnapshot.VariantOkOf(patient, GetVariantID(), 4) == 0)
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
		OMD_IDS_ExtractWork.BeginShock(action_data, 0);
	}

	override void OnUpdateServer(ActionData action_data)
	{
		super.OnUpdateServer(action_data);
		OMD_IDS_ExtractWork.TickShock(action_data, 0, 0);
	}

	override void OnEndServer(ActionData action_data)
	{
		OMD_IDS_ExtractWork.EndShock(action_data, 0);
		super.OnEndServer(action_data);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		OMD_IDS_ExtractWork.Finish(action_data, 0);
	}
};

modded class HuntingKnife
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(OMD_IDS_ActionExtractBulletSelf);
		AddAction(OMD_IDS_ActionExtractBulletTarget);
	}
};

modded class PlayerBase
{
	protected int m_OMD_IDS_BoneBreakSeq;
	protected int m_OMD_IDS_BleedLight;
	protected int m_OMD_IDS_BleedHeavy;
	// Packed hand treatment. Not saved. Rebuilt from records and effects.
	protected int m_OMD_IDS_TreatLeft;
	protected int m_OMD_IDS_TreatRight;
	// Torso treatment, same 16-bit slot as a hand. Not saved. Rebuilt with the hand ints.
	// A bits 0-15 UpperChest, bits 16-31 MiddleChest.
	// B bits 0-15 Pelvis, bits 16-31 LeftShoulder.
	// C bits 0-15 RightShoulder, bits 16-31 stay 0.
	protected int m_OMD_IDS_TreatTorsoA;
	protected int m_OMD_IDS_TreatTorsoB;
	protected int m_OMD_IDS_TreatTorsoC;
	// Closed pelvis fracture: 1 limits erect and crouch to walk. Not saved.
	protected int m_OMD_IDS_PelvisSlow;
	// True after this machine applied that walk cap. Not NetSync.
	protected bool m_OMD_IDS_PelvisSlowHeld;
	protected ref OMD_IDS_BoneBreakSoundClient m_OMD_IDS_BoneBreakSound;
	protected ref OMD_IDS_BleedFx m_OMD_IDS_BleedFx;
	protected ref OMD_IDS_PainEffectState m_OMD_IDS_PainFx;
	// Hand-action variant for the dedicated server check. Not NetSync and not saved.
	protected int m_OMD_IDS_HandVariant;
	// Last PAIN_LEVELS hit serial this client already handled. Not saved and not NetSync.
	protected int m_OMD_IDS_HitFxSeen;

	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableInt("m_OMD_IDS_BoneBreakSeq", 0, 255);
		// Bit masks. No min/max: when they are equal the engine does not quantize.
		// P:\scripts\3_game\entities\entityai.c:2847
		RegisterNetSyncVariableInt("m_OMD_IDS_BleedLight");
		RegisterNetSyncVariableInt("m_OMD_IDS_BleedHeavy");
		// Full words, including bit 31. No min/max: when they are equal the engine does not quantize.
		// P:\scripts\3_game\entities\entityai.c:2847
		RegisterNetSyncVariableInt("m_OMD_IDS_TreatLeft");
		RegisterNetSyncVariableInt("m_OMD_IDS_TreatRight");
		RegisterNetSyncVariableInt("m_OMD_IDS_TreatTorsoA");
		RegisterNetSyncVariableInt("m_OMD_IDS_TreatTorsoB");
		RegisterNetSyncVariableInt("m_OMD_IDS_TreatTorsoC");
		RegisterNetSyncVariableInt("m_OMD_IDS_PelvisSlow", 0, 1);
	}

	override void EEInit()
	{
		super.EEInit();
		OMD_IDS_CaptureBoneBreak();
		if (!GetGame())
		{
			return;
		}

		if (GetGame().IsDedicatedServer())
		{
			return;
		}

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.OMD_IDS_ArmBoneBreak);
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		OMD_IDS_RefreshFx();
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		if (IsControlledPlayer())
		{
			OMD_IDS_PainJerk.Reset();
			OMD_IDS_PainStore.Get().Replace(0, null, null);
			if (m_OMD_IDS_PainFx)
			{
				m_OMD_IDS_PainFx.Clear();
			}
		}

		OMD_IDS_RefreshFx();
	}

	override void EEDelete(EntityAI parent)
	{
		if (IsControlledPlayer())
		{
			OMD_IDS_PainJerk.Reset();
			OMD_IDS_PainStore.Get().Replace(0, null, null);
			if (m_OMD_IDS_PainFx)
			{
				m_OMD_IDS_PainFx.Clear();
			}
		}

		OMD_IDS_DropBleed();
		super.EEDelete(parent);
	}

	override void OnSimulationDisabled()
	{
		OMD_IDS_DropBleed();
		super.OnSimulationDisabled();
	}

	override void OnSimulationEnabled()
	{
		super.OnSimulationEnabled();
		OMD_IDS_RefreshFx();
	}

	void OMD_IDS_AdvanceBoneBreakSeq()
	{
		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		m_OMD_IDS_BoneBreakSeq = m_OMD_IDS_BoneBreakSeq + 1;
		if (m_OMD_IDS_BoneBreakSeq > 255)
		{
			m_OMD_IDS_BoneBreakSeq = 0;
		}

		SetSynchDirty();
	}

	int OMD_IDS_GetBoneBreakSeq()
	{
		return m_OMD_IDS_BoneBreakSeq;
	}

	protected void OMD_IDS_CaptureBoneBreak()
	{
		if (!m_OMD_IDS_BoneBreakSound)
		{
			m_OMD_IDS_BoneBreakSound = new OMD_IDS_BoneBreakSoundClient();
		}

		m_OMD_IDS_BoneBreakSound.Remember(m_OMD_IDS_BoneBreakSeq);
	}

	void OMD_IDS_ArmBoneBreak()
	{
		if (!m_OMD_IDS_BoneBreakSound)
		{
			m_OMD_IDS_BoneBreakSound = new OMD_IDS_BoneBreakSoundClient();
		}

		m_OMD_IDS_BoneBreakSound.GoLive(m_OMD_IDS_BoneBreakSeq);
	}

	void OMD_IDS_SetBleedMasks(int lightMask, int heavyMask)
	{
		bool changed;

		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsServer())
		{
			return;
		}

		changed = false;
		if (m_OMD_IDS_BleedLight != lightMask)
		{
			m_OMD_IDS_BleedLight = lightMask;
			changed = true;
		}

		if (m_OMD_IDS_BleedHeavy != heavyMask)
		{
			m_OMD_IDS_BleedHeavy = heavyMask;
			changed = true;
		}

		if (!changed)
		{
			return;
		}

		SetSynchDirty();
	}

	int OMD_IDS_GetTreatLeft()
	{
		return m_OMD_IDS_TreatLeft;
	}

	int OMD_IDS_GetTreatRight()
	{
		return m_OMD_IDS_TreatRight;
	}

	int OMD_IDS_GetTreatTorsoA()
	{
		return m_OMD_IDS_TreatTorsoA;
	}

	int OMD_IDS_GetTreatTorsoB()
	{
		return m_OMD_IDS_TreatTorsoB;
	}

	int OMD_IDS_GetTreatTorsoC()
	{
		return m_OMD_IDS_TreatTorsoC;
	}

	int OMD_IDS_GetPelvisSlow()
	{
		return m_OMD_IDS_PelvisSlow;
	}

	// super runs first. A true result from another mod is returned as-is.
	// P:\scripts\4_world\entities\dayzplayerimplement.c:2276
	override bool ModCommandHandlerAfter(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		bool handled;

		handled = super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished);
		OMD_IDS_UpdatePelvisSlow();
		return handled;
	}

	// Walk cap while the pelvis flag is 1. The latch drops when the flag is 0 or the player is down or surrendering.
	// Set*SpeedLimit(false) runs only when surrender does not own the limits.
	// Broken legs do not call SetErectSpeedLimit. P:\scripts\4_world\entities\manbase\playerbase.c:2057
	protected void OMD_IDS_UpdatePelvisSlow()
	{
		HumanInputController hic;
		int wantSlow;
		int surrenderHolds;

		wantSlow = 0;
		surrenderHolds = 0;
		if (m_OMD_IDS_PelvisSlow == 1)
		{
			if (!OMD_IDS_PelvisSlowBlocked())
			{
				wantSlow = 1;
			}
		}

		if (IsSurrendered())
		{
			surrenderHolds = 1;
		}

		if (m_StartedSurrenderTransition)
		{
			surrenderHolds = 1;
		}

		hic = GetInputController();
		if (!hic)
		{
			return;
		}

		if (wantSlow == 1)
		{
			if (m_OMD_IDS_PelvisSlowHeld)
			{
				return;
			}

			hic.SetErectSpeedLimit(true, DayZPlayerConstants.MOVEMENTIDX_WALK);
			hic.SetCrouchSpeedLimit(true, DayZPlayerConstants.MOVEMENTIDX_WALK);
			m_OMD_IDS_PelvisSlowHeld = true;
			return;
		}

		if (!m_OMD_IDS_PelvisSlowHeld)
		{
			return;
		}

		m_OMD_IDS_PelvisSlowHeld = false;
		if (surrenderHolds == 1)
		{
			return;
		}

		hic.SetErectSpeedLimit(false);
		hic.SetCrouchSpeedLimit(false);
	}

	// Faint uses RequestUnconsciousness, so the vanilla uncon request covers it.
	// IsUnconscious stays false until the command starts. P:\scripts\4_world\entities\manbase\playerbase.c:3925
	protected bool OMD_IDS_PelvisSlowBlocked()
	{
		if (m_ShouldBeUnconscious)
		{
			return true;
		}

		if (IsUnconscious())
		{
			return true;
		}

		if (GetCommand_Unconscious())
		{
			return true;
		}

		if (IsSurrendered())
		{
			return true;
		}

		if (m_StartedSurrenderTransition)
		{
			return true;
		}

		return false;
	}

	protected void OMD_IDS_RefreshFx()
	{
		if (!GetGame())
		{
			return;
		}

		if (GetGame().IsDedicatedServer())
		{
			return;
		}

		if (!m_OMD_IDS_BoneBreakSound)
		{
			m_OMD_IDS_BoneBreakSound = new OMD_IDS_BoneBreakSoundClient();
		}

		m_OMD_IDS_BoneBreakSound.OnSynced(this, m_OMD_IDS_BoneBreakSeq);
		if (!m_OMD_IDS_BleedFx)
		{
			m_OMD_IDS_BleedFx = new OMD_IDS_BleedFx();
		}

		m_OMD_IDS_BleedFx.Apply(this, m_OMD_IDS_BleedLight, m_OMD_IDS_BleedHeavy);
	}

	protected void OMD_IDS_DropBleed()
	{
		if (!m_OMD_IDS_BleedFx)
		{
			return;
		}

		m_OMD_IDS_BleedFx.Drop();
	}

	// Client OnSelectPlayer calls this after the cfg gameplay packet is already in CfgGameplayHandler.m_Data.
	// P:\scripts\4_world\entities\manbase\playerbase.c:6413
	// P:\scripts\3_game\entities\man.c:867
	override void OnGameplayDataHandlerSync()
	{
		super.OnGameplayDataHandlerSync();
		OMD_IDS_ApplyTreatSync();
		OMD_IDS_LogArrayProbe();
	}

	protected void OMD_IDS_ApplyTreatSync()
	{
		OMD_IDS_TreatSync packet;

		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsClient())
		{
			return;
		}

		packet = CfgGameplayHandler.GetOMD_IDS_Treat();
		if (!packet)
		{
			Print("[OMD_IDS][CfgSync] client ready=0 extract=0 suture=0 remove=0 zones splint=0 suture=0 dressing=0 extract=0 remove=0 probe=0 arraysNull=5.");
			return;
		}

		packet.ApplyOnClient();
	}

	// Diagnostic only. Reads the vanilla string array and the treat packet. Does not apply them.
	protected void OMD_IDS_LogArrayProbe()
	{
		int count;
		int index;
		int ready;
		int probe;
		int sum;
		string items;
		string line;
		TStringArray files;
		OMD_IDS_TreatSync packet;

		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsClient())
		{
			return;
		}

		count = -1;
		items = "";
		if (CfgGameplayHandler.m_Data)
		{
			if (CfgGameplayHandler.m_Data.PlayerData)
			{
				files = CfgGameplayHandler.m_Data.PlayerData.spawnGearPresetFiles;
				if (files)
				{
					count = files.Count();
					for (index = 0; index < count; index = index + 1)
					{
						if (items != "")
						{
							items = items + " ";
						}

						items = items + files.Get(index);
					}
				}
			}
		}

		line = "[OMD_IDS][ArrayProbe] client vanilla spawnGear count=" + count.ToString() + " items=" + items;
		Print(line);
		packet = CfgGameplayHandler.GetOMD_IDS_Treat();
		if (!packet)
		{
			return;
		}

		ready = packet.m_Ready;
		probe = packet.zonesProbe;
		sum = 0;
		if (packet.splint)
		{
			sum = sum + packet.splint.Count();
		}

		if (packet.suture)
		{
			sum = sum + packet.suture.Count();
		}

		if (packet.dressing)
		{
			sum = sum + packet.dressing.Count();
		}

		if (packet.extractBullet)
		{
			sum = sum + packet.extractBullet.Count();
		}

		if (packet.removeDressing)
		{
			sum = sum + packet.removeDressing.Count();
		}

		line = string.Format("[OMD_IDS][ArrayProbe] client treat ready=%1 probe=%2 sum=%3", ready, probe, sum);
		Print(line);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		if (rpc_type == OMD_IDS_Rpc.PAIN_LEVELS)
		{
			OMD_IDS_ReadPainLevels(ctx);
			return;
		}

		if (rpc_type != OMD_IDS_Rpc.SNAPSHOT)
		{
			return;
		}

		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsClient())
		{
			return;
		}

		int count;
		if (!ctx.Read(count))
		{
			Print("[OMD_IDS] injury snapshot was not read");
			return;
		}

		if (count < 0)
		{
			Print("[OMD_IDS] injury snapshot count is invalid");
			return;
		}

		if (count > OMD_IDS_Rpc.SNAPSHOT_MAX)
		{
			Print("[OMD_IDS] injury snapshot is too large");
			return;
		}

		array<ref OMD_IDS_InjurySnapshotEntry> entries = new array<ref OMD_IDS_InjurySnapshotEntry>();
		int index;
		int typeId;
		int groupId;
		int entryCount;
		OMD_IDS_InjurySnapshotEntry entry;
		for (index = 0; index < count; index = index + 1)
		{
			if (!ctx.Read(typeId))
			{
				Print("[OMD_IDS] injury snapshot was cut short");
				return;
			}

			if (!ctx.Read(groupId))
			{
				Print("[OMD_IDS] injury snapshot was cut short");
				return;
			}

			if (!ctx.Read(entryCount))
			{
				Print("[OMD_IDS] injury snapshot was cut short");
				return;
			}

			entry = new OMD_IDS_InjurySnapshotEntry();
			entry.typeId = typeId;
			entry.groupId = groupId;
			entry.count = entryCount;
			entries.Insert(entry);
		}

		Print(string.Format("[OMD_IDS] client snapshot entries=%1", entries.Count()));
		OMD_IDS_InjuryMailbox.Get().Put(entries);
	}

	OMD_IDS_PainEffectState OMD_IDS_GetPainFx()
	{
		if (!m_OMD_IDS_PainFx)
		{
			m_OMD_IDS_PainFx = new OMD_IDS_PainEffectState();
		}

		return m_OMD_IDS_PainFx;
	}

	protected void OMD_IDS_ReadPainLevels(ParamsReadContext ctx)
	{
		int bodyLevel;
		int count;
		int index;
		int zoneId;
		int level;
		int hitFxCode;
		int hitSerial;
		array<int> zoneIds;
		array<int> levels;
		OMD_IDS_HitHookClient hitHook;

		if (!GetGame())
		{
			return;
		}

		if (!GetGame().IsClient())
		{
			return;
		}

		if (!ctx.Read(bodyLevel))
		{
			Print("[OMD_IDS][Pain] client levels were not read");
			return;
		}

		if (!ctx.Read(count))
		{
			Print("[OMD_IDS][Pain] client levels were not read");
			return;
		}

		if (count < 0)
		{
			Print("[OMD_IDS][Pain] client levels count is invalid");
			return;
		}

		if (count > OMD_IDS_ZoneId.GUTS)
		{
			Print("[OMD_IDS][Pain] client levels count is invalid");
			return;
		}

		zoneIds = new array<int>();
		levels = new array<int>();
		for (index = 0; index < count; index = index + 1)
		{
			if (!ctx.Read(zoneId))
			{
				Print("[OMD_IDS][Pain] client levels were cut short");
				return;
			}

			if (!ctx.Read(level))
			{
				Print("[OMD_IDS][Pain] client levels were cut short");
				return;
			}

			zoneIds.Insert(zoneId);
			levels.Insert(level);
		}

		if (!OMD_IDS_GetPainFx().ReadRpc(ctx))
		{
			Print("[OMD_IDS][Pain] client effect numbers were not read");
			return;
		}

		hitFxCode = OMD_IDS_GetPainFx().GetHitFxCode();
		hitSerial = OMD_IDS_GetPainFx().GetHitSerial();
		if (IsControlledPlayer())
		{
			if (hitSerial != m_OMD_IDS_HitFxSeen)
			{
				m_OMD_IDS_HitFxSeen = hitSerial;
				hitHook = new OMD_IDS_HitHookClient();
				hitHook.OnHitFx(this, hitFxCode, hitSerial);
			}
		}

		OMD_IDS_PainStore.Get().Replace(bodyLevel, zoneIds, levels);
		Print(string.Format("[OMD_IDS][Pain] client body=%1 zones=%2", bodyLevel, count));
	}

	void OMD_IDS_SetHandVariant(int variantId)
	{
		m_OMD_IDS_HandVariant = variantId;
		if (m_OMD_IDS_HandVariant < 0)
		{
			m_OMD_IDS_HandVariant = 0;
		}
	}

	int OMD_IDS_GetHandVariant()
	{
		return m_OMD_IDS_HandVariant;
	}
};

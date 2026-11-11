modded class PlayerBase
{
	protected int m_OMD_IDS_BoneBreakSeq;
	protected int m_OMD_IDS_BleedLight;
	protected int m_OMD_IDS_BleedHeavy;
	// Packed hand treatment. Not saved. Rebuilt from records and effects.
	protected int m_OMD_IDS_TreatLeft;
	protected int m_OMD_IDS_TreatRight;
	protected ref OMD_IDS_BoneBreakSoundClient m_OMD_IDS_BoneBreakSound;
	protected ref OMD_IDS_BleedFx m_OMD_IDS_BleedFx;
	protected ref OMD_IDS_PainEffectState m_OMD_IDS_PainFx;
	// Hand-action variant for the dedicated server check. Not NetSync and not saved.
	protected int m_OMD_IDS_HandVariant;

	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableInt("m_OMD_IDS_BoneBreakSeq", 0, 255);
		// Bit masks. No min/max: when they are equal the engine does not quantize.
		// P:\scripts\3_game\entities\entityai.c:2847
		RegisterNetSyncVariableInt("m_OMD_IDS_BleedLight");
		RegisterNetSyncVariableInt("m_OMD_IDS_BleedHeavy");
		// Bitmask, bits 0-15. No min/max: when they are equal the engine does not quantize.
		// P:\scripts\3_game\entities\entityai.c:2847
		RegisterNetSyncVariableInt("m_OMD_IDS_TreatLeft");
		RegisterNetSyncVariableInt("m_OMD_IDS_TreatRight");
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
		array<int> zoneIds;
		array<int> levels;

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

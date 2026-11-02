modded class PlayerBase
{
	protected int m_OMD_IDS_BoneBreakSeq;
	protected int m_OMD_IDS_BleedLight;
	protected int m_OMD_IDS_BleedHeavy;
	protected ref OMD_IDS_BoneBreakSoundClient m_OMD_IDS_BoneBreakSound;
	protected ref OMD_IDS_BleedFx m_OMD_IDS_BleedFx;

	override void Init()
	{
		super.Init();
		RegisterNetSyncVariableInt("m_OMD_IDS_BoneBreakSeq", 0, 255);
		RegisterNetSyncVariableInt("m_OMD_IDS_BleedLight", 0, 2147483647);
		RegisterNetSyncVariableInt("m_OMD_IDS_BleedHeavy", 0, 2147483647);
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
		OMD_IDS_RefreshFx();
	}

	override void EEDelete(EntityAI parent)
	{
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

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
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
};

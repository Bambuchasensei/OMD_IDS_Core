modded class PlayerBase
{
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

		OMD_IDS_InjuryMailbox.Get().Put(entries);
	}
};

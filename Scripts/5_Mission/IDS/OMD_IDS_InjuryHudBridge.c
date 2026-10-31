#ifndef SERVER

class OMD_IDS_InjuryHudBridge
{
	static array<ref OMD_IDS_HudDamageEntry> ToEntries(array<ref OMD_IDS_InjurySnapshotEntry> snapshot)
	{
		array<ref OMD_IDS_HudDamageEntry> result = new array<ref OMD_IDS_HudDamageEntry>();
		if (!snapshot)
		{
			return result;
		}

		int index;
		for (index = 0; index < snapshot.Count(); index = index + 1)
		{
			OMD_IDS_InjurySnapshotEntry source = snapshot.Get(index);
			if (!source)
			{
				continue;
			}

			if (source.count < 1)
			{
				continue;
			}

			string groupId = OMD_IDS_GroupCatalog.GetName(source.groupId);
			if (groupId == "")
			{
				Print(string.Format("[OMD_IDS] injury snapshot group %1 has no name", source.groupId));
				continue;
			}

			OMD_IDS_HudDamageEntry entry = new OMD_IDS_HudDamageEntry();
			entry.groupId = groupId;
			entry.label = OMD_IDS_InjuryTypeCatalog.GetLabel(source.typeId);
			entry.count = source.count;
			result.Insert(entry);
		}

		return result;
	}
};

#endif

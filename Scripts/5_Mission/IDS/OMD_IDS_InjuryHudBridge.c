#ifndef SERVER

class OMD_IDS_InjuryHudBridge
{
	static array<ref OMD_IDS_HudDamageEntry> ToEntries(array<ref OMD_IDS_InjurySnapshotEntry> snapshot)
	{
		array<ref OMD_IDS_HudDamageEntry> result;
		array<ref OMD_IDS_HudDamageEntry> injuries;
		array<ref OMD_IDS_HudDamageEntry> treatments;
		int index;
		int place;
		string groupName;
		OMD_IDS_InjurySnapshotEntry source;
		OMD_IDS_HudDamageEntry entry;
		OMD_IDS_HudDamageEntry treatment;

		result = new array<ref OMD_IDS_HudDamageEntry>();
		injuries = new array<ref OMD_IDS_HudDamageEntry>();
		treatments = new array<ref OMD_IDS_HudDamageEntry>();
		if (!snapshot)
		{
			return result;
		}

		for (index = 0; index < snapshot.Count(); index = index + 1)
		{
			source = snapshot.Get(index);
			if (!source)
			{
				continue;
			}

			if (source.count < 1)
			{
				continue;
			}

			if (!OMD_IDS_InjuryTypeCatalog.IsKnown(source.typeId))
			{
				continue;
			}

			groupName = OMD_IDS_GroupCatalog.GetName(source.groupId);
			if (groupName == "")
			{
				// This build has no name for the group. Skip the row. Do not log.
				continue;
			}

			if (SkipReplaced(snapshot, source) != 0)
			{
				continue;
			}

			entry = new OMD_IDS_HudDamageEntry();
			entry.groupId = groupName;
			entry.label = OMD_IDS_InjuryTypeCatalog.GetLabel(source.typeId);
			entry.count = source.count;
			if (IsTreatmentType(source.typeId) != 0)
			{
				treatments.Insert(entry);
			}
			else
			{
				injuries.Insert(entry);
			}
		}

		for (index = 0; index < injuries.Count(); index = index + 1)
		{
			entry = injuries.Get(index);
			if (!entry)
			{
				continue;
			}

			result.Insert(entry);
		}

		for (index = 0; index < treatments.Count(); index = index + 1)
		{
			treatment = treatments.Get(index);
			if (!treatment)
			{
				continue;
			}

			place = LastGroupIndex(result, treatment.groupId);
			if (place < 0)
			{
				result.Insert(treatment);
			}
			else
			{
				if (place + 1 >= result.Count())
				{
					result.Insert(treatment);
				}
				else
				{
					result.InsertAt(treatment, place + 1);
				}
			}
		}

		return result;
	}

	private static int IsTreatmentType(int typeId)
	{
		if (typeId == OMD_IDS_InjuryTypeId.SUTURED)
		{
			return 1;
		}

		if (typeId == OMD_IDS_InjuryTypeId.DRESSING_CLEAN)
		{
			return 1;
		}

		if (typeId == OMD_IDS_InjuryTypeId.DRESSING_DIRTY)
		{
			return 1;
		}

		if (typeId == OMD_IDS_InjuryTypeId.SPLINTED)
		{
			return 1;
		}

		return 0;
	}

	// Dressing replaces the wound and the suture on this group. Suture replaces the wound.
	// A splint replaces the bone mark. The snapshot rows stay. Only the drawn badge changes.
	private static int SkipReplaced(array<ref OMD_IDS_InjurySnapshotEntry> snapshot, OMD_IDS_InjurySnapshotEntry source)
	{
		int dressed;

		dressed = 0;
		if (!source)
		{
			return 0;
		}

		if (HasGroupType(snapshot, source.groupId, OMD_IDS_InjuryTypeId.DRESSING_CLEAN) != 0)
		{
			dressed = 1;
		}

		if (HasGroupType(snapshot, source.groupId, OMD_IDS_InjuryTypeId.DRESSING_DIRTY) != 0)
		{
			dressed = 1;
		}

		if (dressed != 0)
		{
			if (IsWoundSlot(source.typeId) != 0)
			{
				return 1;
			}

			if (source.typeId == OMD_IDS_InjuryTypeId.SUTURED)
			{
				return 1;
			}
		}

		if (HasGroupType(snapshot, source.groupId, OMD_IDS_InjuryTypeId.SUTURED) != 0)
		{
			if (IsWoundSlot(source.typeId) != 0)
			{
				return 1;
			}
		}

		if (HasGroupType(snapshot, source.groupId, OMD_IDS_InjuryTypeId.SPLINTED) != 0)
		{
			if (IsBoneSlot(source.typeId) != 0)
			{
				return 1;
			}
		}

		return 0;
	}

	private static int IsWoundSlot(int typeId)
	{
		if (typeId == OMD_IDS_InjuryTypeId.SCRATCH)
		{
			return 1;
		}

		if (typeId == OMD_IDS_InjuryTypeId.LIGHT_WOUND)
		{
			return 1;
		}

		if (typeId == OMD_IDS_InjuryTypeId.DEEP_WOUND)
		{
			return 1;
		}

		if (typeId == OMD_IDS_InjuryTypeId.BIG_WOUND)
		{
			return 1;
		}

		return 0;
	}

	private static int IsBoneSlot(int typeId)
	{
		if (typeId == OMD_IDS_InjuryTypeId.BONE_BRUISE)
		{
			return 1;
		}

		if (typeId == OMD_IDS_InjuryTypeId.STRESS_FRACTURE)
		{
			return 1;
		}

		if (typeId == OMD_IDS_InjuryTypeId.CLOSED_FRACTURE)
		{
			return 1;
		}

		if (typeId == OMD_IDS_InjuryTypeId.OPEN_FRACTURE)
		{
			return 1;
		}

		return 0;
	}

	private static int HasGroupType(array<ref OMD_IDS_InjurySnapshotEntry> snapshot, int groupId, int typeId)
	{
		int index;
		OMD_IDS_InjurySnapshotEntry row;

		if (!snapshot)
		{
			return 0;
		}

		for (index = 0; index < snapshot.Count(); index = index + 1)
		{
			row = snapshot.Get(index);
			if (!row)
			{
				continue;
			}

			if (row.groupId != groupId)
			{
				continue;
			}

			if (row.typeId != typeId)
			{
				continue;
			}

			if (row.count < 1)
			{
				continue;
			}

			return 1;
		}

		return 0;
	}

	private static int LastGroupIndex(array<ref OMD_IDS_HudDamageEntry> rows, string groupId)
	{
		int index;
		int found;
		OMD_IDS_HudDamageEntry row;

		found = -1;
		if (!rows)
		{
			return found;
		}

		for (index = 0; index < rows.Count(); index = index + 1)
		{
			row = rows.Get(index);
			if (!row)
			{
				continue;
			}

			if (row.groupId != groupId)
			{
				continue;
			}

			found = index;
		}

		return found;
	}
};

#endif

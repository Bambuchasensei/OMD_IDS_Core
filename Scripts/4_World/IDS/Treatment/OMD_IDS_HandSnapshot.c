// Client read of the owner injury snapshot.
// LeftHand and RightHand are separate groups. A row count is that hand only.
// HP is not in the snapshot. Variants follow catalog order, the same list the server builds.
// One ready hand: variant 0 is that hand. Both ready: variant 0 is left, variant 1 is right.
class OMD_IDS_HandSnapshot
{
	static int WoundCount()
	{
		return Count(OMD_IDS_InjuryTypeId.DEEP_WOUND, OMD_IDS_InjuryTypeId.BIG_WOUND);
	}

	static int FractureCount()
	{
		return Count(OMD_IDS_InjuryTypeId.CLOSED_FRACTURE, OMD_IDS_InjuryTypeId.OPEN_FRACTURE);
	}

	// Scratch and every heavier wound stage on either hand. Not a fracture by itself.
	static int BandageCount()
	{
		return CountWounds();
	}

	static PlayerBase OtherPlayer(ActionTarget target)
	{
		PlayerBase patient;

		patient = null;
		if (!target)
		{
			return patient;
		}

		if (!Class.CastTo(patient, target.GetObject()))
		{
			return null;
		}

		return patient;
	}

	static int VariantOf(ActionBase action, PlayerBase actor)
	{
		int variantId;

		variantId = 0;
		if (action)
		{
			variantId = action.GetVariantID();
		}

		if (!GetGame())
		{
			return variantId;
		}

		if (!GetGame().IsDedicatedServer())
		{
			return variantId;
		}

		if (!actor)
		{
			return variantId;
		}

		return actor.OMD_IDS_GetHandVariant();
	}

	private static int Count(int typeA, int typeB)
	{
		int index;
		int total;
		OMD_IDS_InjuryMailbox mailbox;
		array<ref OMD_IDS_InjurySnapshotEntry> rows;
		OMD_IDS_InjurySnapshotEntry row;

		total = 0;
		mailbox = OMD_IDS_InjuryMailbox.Get();
		if (!mailbox)
		{
			return total;
		}

		rows = mailbox.Current();
		if (!rows)
		{
			return total;
		}

		for (index = 0; index < rows.Count(); index = index + 1)
		{
			row = rows.Get(index);
			if (!row)
			{
				continue;
			}

			if (IsHandGroup(row.groupId) == 0)
			{
				continue;
			}

			if (row.typeId != typeA)
			{
				if (row.typeId != typeB)
				{
					continue;
				}
			}

			if (row.count > 0)
			{
				total = total + row.count;
			}
		}

		return total;
	}

	private static int CountWounds()
	{
		int index;
		int total;
		OMD_IDS_InjuryMailbox mailbox;
		array<ref OMD_IDS_InjurySnapshotEntry> rows;
		OMD_IDS_InjurySnapshotEntry row;

		total = 0;
		mailbox = OMD_IDS_InjuryMailbox.Get();
		if (!mailbox)
		{
			return total;
		}

		rows = mailbox.Current();
		if (!rows)
		{
			return total;
		}

		for (index = 0; index < rows.Count(); index = index + 1)
		{
			row = rows.Get(index);
			if (!row)
			{
				continue;
			}

			if (IsHandGroup(row.groupId) == 0)
			{
				continue;
			}

			if (row.typeId != OMD_IDS_InjuryTypeId.SCRATCH)
			{
				if (row.typeId != OMD_IDS_InjuryTypeId.LIGHT_WOUND)
				{
					if (row.typeId != OMD_IDS_InjuryTypeId.DEEP_WOUND)
					{
						if (row.typeId != OMD_IDS_InjuryTypeId.BIG_WOUND)
						{
							continue;
						}
					}
				}
			}

			if (row.count > 0)
			{
				total = total + row.count;
			}
		}

		return total;
	}

	static int SutureOfferCount()
	{
		return HandsReady(0);
	}

	static int BandageOfferCount()
	{
		return HandsReady(2);
	}

	static int SplintOfferCount()
	{
		return HandsReady(1);
	}

	static int RemoveDressingOfferCount()
	{
		return HandsReady(3);
	}

	static int ExtractOfferCount()
	{
		return HandsReady(4);
	}

	// mode 0 suture, 1 splint, 2 bandage, 3 remove dressing, 4 extract bullet. Same numbers as server PickHand.
	// The result is how many catalog zones qualify, not which one variant 0 hits.
	private static int HandsReady(int mode)
	{
		int index;
		int total;
		int zoneId;
		int groupId;

		total = 0;
		for (index = 0; index < OMD_IDS_LimbCatalog.Count(); index = index + 1)
		{
			zoneId = OMD_IDS_LimbCatalog.ZoneAt(index);
			groupId = OMD_IDS_LimbCatalog.HudGroup(zoneId);
			if (HandReady(groupId, mode) != 0)
			{
				total = total + 1;
			}
		}

		return total;
	}

	private static int HandReady(int groupId, int mode)
	{
		int zoneId;

		zoneId = OMD_IDS_LimbCatalog.ZoneByHudGroup(groupId);
		if (zoneId == 0)
		{
			return 0;
		}

		if (OMD_IDS_TreatAllow.IsAllowed(zoneId, OMD_IDS_TreatAllow.TypeOfMode(mode)) == 0)
		{
			return 0;
		}

		if (mode == 4)
		{
			if (HasRow(groupId, OMD_IDS_InjuryTypeId.BULLET) == 0)
			{
				return 0;
			}

			if (HasRow(groupId, OMD_IDS_InjuryTypeId.DRESSING_CLEAN) != 0)
			{
				return 0;
			}

			if (HasRow(groupId, OMD_IDS_InjuryTypeId.DRESSING_DIRTY) != 0)
			{
				return 0;
			}

			return 1;
		}

		if (mode == 1)
		{
			if (HasRow(groupId, OMD_IDS_InjuryTypeId.CLOSED_FRACTURE) == 0)
			{
				if (HasRow(groupId, OMD_IDS_InjuryTypeId.OPEN_FRACTURE) == 0)
				{
					return 0;
				}
			}

			if (HasRow(groupId, OMD_IDS_InjuryTypeId.SPLINTED) != 0)
			{
				return 0;
			}

			return 1;
		}

		if (mode == 2)
		{
			if (HasWound(groupId) == 0)
			{
				return 0;
			}

			if (HasRow(groupId, OMD_IDS_InjuryTypeId.DRESSING_CLEAN) != 0)
			{
				return 0;
			}

			return 1;
		}

		if (mode == 3)
		{
			if (HasRow(groupId, OMD_IDS_InjuryTypeId.DRESSING_CLEAN) != 0)
			{
				return 1;
			}

			if (HasRow(groupId, OMD_IDS_InjuryTypeId.DRESSING_DIRTY) != 0)
			{
				return 1;
			}

			return 0;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.DEEP_WOUND) == 0)
		{
			if (HasRow(groupId, OMD_IDS_InjuryTypeId.BIG_WOUND) == 0)
			{
				return 0;
			}
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.SUTURED) != 0)
		{
			return 0;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.DRESSING_CLEAN) != 0)
		{
			return 0;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.DRESSING_DIRTY) != 0)
		{
			return 0;
		}

		return 1;
	}

	private static int IsHandGroup(int groupId)
	{
		if (OMD_IDS_LimbCatalog.ZoneByHudGroup(groupId) == 0)
		{
			return 0;
		}

		return 1;
	}

	private static int HasWound(int groupId)
	{
		if (HasRow(groupId, OMD_IDS_InjuryTypeId.SCRATCH) != 0)
		{
			return 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.LIGHT_WOUND) != 0)
		{
			return 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.DEEP_WOUND) != 0)
		{
			return 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.BIG_WOUND) != 0)
		{
			return 1;
		}

		return 0;
	}

	private static int HasRow(int groupId, int typeId)
	{
		int index;
		OMD_IDS_InjuryMailbox mailbox;
		array<ref OMD_IDS_InjurySnapshotEntry> rows;
		OMD_IDS_InjurySnapshotEntry row;

		mailbox = OMD_IDS_InjuryMailbox.Get();
		if (!mailbox)
		{
			return 0;
		}

		rows = mailbox.Current();
		if (!rows)
		{
			return 0;
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

			if (row.typeId != typeId)
			{
				continue;
			}

			if (row.count > 0)
			{
				return 1;
			}
		}

		return 0;
	}

	// Raw snapshot rows for the actor's own hands. HUD hiding does not remove the row.
	static void ActionFlags(int groupId, out int deep, out int fracture, out int bullet, out int dressing, out int suture, out int splint)
	{
		deep = 0;
		fracture = 0;
		bullet = 0;
		dressing = 0;
		suture = 0;
		splint = 0;
		if (HasRow(groupId, OMD_IDS_InjuryTypeId.DEEP_WOUND) != 0)
		{
			deep = 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.BIG_WOUND) != 0)
		{
			deep = 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.CLOSED_FRACTURE) != 0)
		{
			fracture = 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.OPEN_FRACTURE) != 0)
		{
			fracture = 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.BULLET) != 0)
		{
			bullet = 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.DRESSING_CLEAN) != 0)
		{
			dressing = 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.DRESSING_DIRTY) != 0)
		{
			dressing = 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.SUTURED) != 0)
		{
			suture = 1;
		}

		if (HasRow(groupId, OMD_IDS_InjuryTypeId.SPLINTED) != 0)
		{
			splint = 1;
		}
	}

	// Zone id for this variant, or 0 when that slot is empty. Same order as OfferGroup.
	static int OfferZone(int variantId, int mode)
	{
		return OMD_IDS_LimbCatalog.ZoneByHudGroup(OfferGroup(variantId, mode));
	}

	// Group id for this variant, or 0 when that slot is empty.
	// One ready hand: variant 0 is that hand. Both ready: variant 0 is left.
	static int OfferGroup(int variantId, int mode)
	{
		int index;
		int seen;
		int zoneId;
		int groupId;

		if (variantId < 0)
		{
			return 0;
		}

		seen = 0;
		for (index = 0; index < OMD_IDS_LimbCatalog.Count(); index = index + 1)
		{
			zoneId = OMD_IDS_LimbCatalog.ZoneAt(index);
			groupId = OMD_IDS_LimbCatalog.HudGroup(zoneId);
			if (HandReady(groupId, mode) == 0)
			{
				continue;
			}

			if (seen == variantId)
			{
				return groupId;
			}

			seen = seen + 1;
		}

		return 0;
	}

	// Empty when the owner snapshot cannot name one hand. The action keeps the neutral text.
	static string NamedBandage(int variantId)
	{
		int groupId;
		int dirty;
		int sutured;

		groupId = OfferGroup(variantId, 2);
		if (groupId == 0)
		{
			return "";
		}

		dirty = HasRow(groupId, OMD_IDS_InjuryTypeId.DRESSING_DIRTY);
		sutured = HasRow(groupId, OMD_IDS_InjuryTypeId.SUTURED);
		return BandageText(groupId, dirty, sutured);
	}

	static string NamedSuture(int variantId)
	{
		return SutureText(OfferGroup(variantId, 0));
	}

	static string NamedSplint(int variantId)
	{
		return SplintText(OfferGroup(variantId, 1));
	}

	static string NamedExtract(int variantId)
	{
		return ExtractText(OfferGroup(variantId, 4));
	}

	static int OfferCountOf(PlayerBase patient, int mode)
	{
		array<int> flags;

		if (!patient)
		{
			return 0;
		}

		if (!patient.IsAlive())
		{
			return 0;
		}

		flags = new array<int>();
		OMD_IDS_LimbCatalog.CollectFlags(patient.OMD_IDS_GetTreatLeft(), patient.OMD_IDS_GetTreatRight(), mode, flags);
		return OMD_IDS_LimbCatalog.ReadyCount(flags, mode);
	}

	static int VariantOkOf(PlayerBase patient, int variantId, int mode)
	{
		if (GroupOf(patient, variantId, mode) == 0)
		{
			return 0;
		}

		return 1;
	}

	static string NamedBandageOf(PlayerBase patient, int variantId)
	{
		int groupId;
		int flags;
		int dirty;
		int sutured;

		groupId = GroupOf(patient, variantId, 2);
		if (groupId == 0)
		{
			return "";
		}

		flags = FlagsFor(patient, groupId);
		dirty = 0;
		if (OMD_IDS_HandTreatFlags.Dressing(flags) == 2)
		{
			dirty = 1;
		}

		sutured = OMD_IDS_HandTreatFlags.HasSuture(flags);
		return BandageText(groupId, dirty, sutured);
	}

	static string NamedSutureOf(PlayerBase patient, int variantId)
	{
		return SutureText(GroupOf(patient, variantId, 0));
	}

	static string NamedSplintOf(PlayerBase patient, int variantId)
	{
		return SplintText(GroupOf(patient, variantId, 1));
	}

	static string NamedExtractOf(PlayerBase patient, int variantId)
	{
		return ExtractText(GroupOf(patient, variantId, 4));
	}

	static string NamedRemoveOf(PlayerBase patient, int variantId)
	{
		return RemoveText(GroupOf(patient, variantId, 3));
	}

	private static int GroupOf(PlayerBase patient, int variantId, int mode)
	{
		int zoneId;
		array<int> flags;

		if (!patient)
		{
			return 0;
		}

		if (!patient.IsAlive())
		{
			return 0;
		}

		flags = new array<int>();
		OMD_IDS_LimbCatalog.CollectFlags(patient.OMD_IDS_GetTreatLeft(), patient.OMD_IDS_GetTreatRight(), mode, flags);
		zoneId = OMD_IDS_LimbCatalog.Pick(flags, variantId, mode);
		return OMD_IDS_LimbCatalog.HudGroup(zoneId);
	}

	private static int FlagsFor(PlayerBase patient, int groupId)
	{
		int zoneId;
		int packed;

		if (!patient)
		{
			return 0;
		}

		zoneId = OMD_IDS_LimbCatalog.ZoneByHudGroup(groupId);
		if (zoneId == 0)
		{
			return 0;
		}

		packed = patient.OMD_IDS_GetTreatLeft();
		if (OMD_IDS_LimbCatalog.SideOf(zoneId) != 0)
		{
			packed = patient.OMD_IDS_GetTreatRight();
		}

		return OMD_IDS_LimbCatalog.FlagWord(packed, zoneId);
	}

	private static string BandageText(int groupId, int dirty, int sutured)
	{
		if (dirty != 0)
		{
			if (groupId == OMD_IDS_GroupId.LEFT_HAND)
			{
				return "Сменить бинт на левой кисти";
			}

			if (groupId == OMD_IDS_GroupId.RIGHT_HAND)
			{
				return "Сменить бинт на правой кисти";
			}

			return OMD_IDS_LimbCatalog.Phrase("Сменить бинт на ", groupId, 1);
		}

		if (groupId == OMD_IDS_GroupId.LEFT_HAND)
		{
			if (sutured != 0)
			{
				return "Забинтовать шов на левой кисти";
			}

			return "Забинтовать рану на левой кисти";
		}

		if (groupId == OMD_IDS_GroupId.RIGHT_HAND)
		{
			if (sutured != 0)
			{
				return "Забинтовать шов на правой кисти";
			}

			return "Забинтовать рану на правой кисти";
		}

		if (sutured != 0)
		{
			return OMD_IDS_LimbCatalog.Phrase("Забинтовать шов на ", groupId, 1);
		}

		return OMD_IDS_LimbCatalog.Phrase("Забинтовать ", groupId, 1);
	}

	private static string SutureText(int groupId)
	{
		if (groupId == OMD_IDS_GroupId.LEFT_HAND)
		{
			return "Зашить рану на левой кисти";
		}

		if (groupId == OMD_IDS_GroupId.RIGHT_HAND)
		{
			return "Зашить рану на правой кисти";
		}

		return OMD_IDS_LimbCatalog.Phrase("Зашить ", groupId, 1);
	}

	private static string SplintText(int groupId)
	{
		if (groupId == OMD_IDS_GroupId.LEFT_HAND)
		{
			return "Наложить шину на левую кисть";
		}

		if (groupId == OMD_IDS_GroupId.RIGHT_HAND)
		{
			return "Наложить шину на правую кисть";
		}

		return OMD_IDS_LimbCatalog.Phrase("Наложить шину на ", groupId, 1);
	}

	private static string ExtractText(int groupId)
	{
		if (groupId == OMD_IDS_GroupId.LEFT_HAND)
		{
			return "Вытащить пулю из левой кисти";
		}

		if (groupId == OMD_IDS_GroupId.RIGHT_HAND)
		{
			return "Вытащить пулю из правой кисти";
		}

		return OMD_IDS_LimbCatalog.Phrase("Вытащить пулю из ", groupId, 0);
	}

	private static string RemoveText(int groupId)
	{
		if (groupId == OMD_IDS_GroupId.LEFT_HAND)
		{
			return "Снять повязку с левой кисти";
		}

		if (groupId == OMD_IDS_GroupId.RIGHT_HAND)
		{
			return "Снять повязку с правой кисти";
		}

		return OMD_IDS_LimbCatalog.Phrase("Снять повязку с ", groupId, 0);
	}

	// 0 is the genitive ("левой кисти"). Any other value is the accusative ("левую кисть").
	static string LimbName(int zoneId, int onto)
	{
		string text;

		text = OMD_IDS_LimbCatalog.Genitive(zoneId);
		if (onto != 0)
		{
			text = OMD_IDS_LimbCatalog.Accusative(zoneId);
		}

		if (text != "")
		{
			return text;
		}

		return OMD_IDS_ZoneCatalog.GetName(zoneId);
	}
};

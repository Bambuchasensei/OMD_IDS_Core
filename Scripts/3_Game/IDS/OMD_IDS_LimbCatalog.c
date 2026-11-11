// Limb profiles the client and the server both know. Cases and HUD groups stay here.
// injuries.json names which of these zones run a mechanic. It does not carry the Russian text.
// Order is the menu order: left then right. A later profile is appended after this one.
class OMD_IDS_LimbZone
{
	string profileId;
	int zoneId;
	int side;
	int hudGroup;
	int bitShift;
	string icon;
	string genitive;
	string accusative;
	string boneSlot;
};

class OMD_IDS_LimbCatalog
{
	private static ref array<ref OMD_IDS_LimbZone> s_Rows;

	private static void Ensure()
	{
		if (s_Rows)
		{
			return;
		}

		s_Rows = new array<ref OMD_IDS_LimbZone>();
		Add("hand", OMD_IDS_ZoneId.LEFT_HAND, 0, OMD_IDS_GroupId.LEFT_HAND, "hand", "левой кисти", "левую кисть", "hand");
		Add("hand", OMD_IDS_ZoneId.RIGHT_HAND, 1, OMD_IDS_GroupId.RIGHT_HAND, "hand", "правой кисти", "правую кисть", "hand");
		Add("foot", OMD_IDS_ZoneId.LEFT_FOOT, 0, OMD_IDS_GroupId.LEFT_FOOT, "foot", "левой ступни", "левую ступню", "foot");
		Add("foot", OMD_IDS_ZoneId.RIGHT_FOOT, 1, OMD_IDS_GroupId.RIGHT_FOOT, "foot", "правой ступни", "правую ступню", "foot");
	}

	private static void Add(string profileId, int zoneId, int side, int hudGroup, string icon, string genitive, string accusative, string boneSlot)
	{
		int index;
		int seen;
		OMD_IDS_LimbZone row;
		OMD_IDS_LimbZone older;

		seen = 0;
		for (index = 0; index < s_Rows.Count(); index = index + 1)
		{
			older = s_Rows.Get(index);
			if (!older)
			{
				continue;
			}

			if (older.side == side)
			{
				seen = seen + 1;
			}
		}

		row = new OMD_IDS_LimbZone();
		row.profileId = profileId;
		row.zoneId = zoneId;
		row.side = side;
		row.hudGroup = hudGroup;
		row.bitShift = seen * 16;
		row.icon = icon;
		row.genitive = genitive;
		row.accusative = accusative;
		row.boneSlot = boneSlot;
		s_Rows.Insert(row);
	}

	private static OMD_IDS_LimbZone FindRow(int zoneId)
	{
		int index;
		OMD_IDS_LimbZone row;

		Ensure();
		for (index = 0; index < s_Rows.Count(); index = index + 1)
		{
			row = s_Rows.Get(index);
			if (!row)
			{
				continue;
			}

			if (row.zoneId == zoneId)
			{
				return row;
			}
		}

		return null;
	}

	static int Count()
	{
		Ensure();
		return s_Rows.Count();
	}

	static int ZoneAt(int index)
	{
		OMD_IDS_LimbZone row;

		Ensure();
		if (index < 0)
		{
			return 0;
		}

		if (index >= s_Rows.Count())
		{
			return 0;
		}

		row = s_Rows.Get(index);
		if (!row)
		{
			return 0;
		}

		return row.zoneId;
	}

	static int HasZone(int zoneId)
	{
		if (!FindRow(zoneId))
		{
			return 0;
		}

		return 1;
	}

	static int SideOf(int zoneId)
	{
		OMD_IDS_LimbZone row;

		row = FindRow(zoneId);
		if (!row)
		{
			return -1;
		}

		return row.side;
	}

	static int HudGroup(int zoneId)
	{
		OMD_IDS_LimbZone row;

		row = FindRow(zoneId);
		if (!row)
		{
			return 0;
		}

		return row.hudGroup;
	}

	static int ZoneByHudGroup(int groupId)
	{
		int index;
		OMD_IDS_LimbZone row;

		Ensure();
		if (groupId == 0)
		{
			return 0;
		}

		for (index = 0; index < s_Rows.Count(); index = index + 1)
		{
			row = s_Rows.Get(index);
			if (!row)
			{
				continue;
			}

			if (row.hudGroup == groupId)
			{
				return row.zoneId;
			}
		}

		return 0;
	}

	static int ZoneOnSide(int side, int slot)
	{
		int index;
		int seen;
		OMD_IDS_LimbZone row;

		Ensure();
		if (slot < 0)
		{
			return 0;
		}

		seen = 0;
		for (index = 0; index < s_Rows.Count(); index = index + 1)
		{
			row = s_Rows.Get(index);
			if (!row)
			{
				continue;
			}

			if (row.side != side)
			{
				continue;
			}

			if (seen == slot)
			{
				return row.zoneId;
			}

			seen = seen + 1;
		}

		return 0;
	}

	static int FlagWord(int packedSide, int zoneId)
	{
		int shift;
		int word;
		OMD_IDS_LimbZone row;

		row = FindRow(zoneId);
		if (!row)
		{
			return 0;
		}

		shift = row.bitShift;
		word = packedSide >> shift;
		word = word & 65535;
		return word;
	}

	static string ProfileId(int zoneId)
	{
		OMD_IDS_LimbZone row;

		row = FindRow(zoneId);
		if (!row)
		{
			return "";
		}

		return row.profileId;
	}

	static string Icon(int zoneId)
	{
		OMD_IDS_LimbZone row;

		row = FindRow(zoneId);
		if (!row)
		{
			return "";
		}

		return row.icon;
	}

	static string BoneSlot(int zoneId)
	{
		OMD_IDS_LimbZone row;

		row = FindRow(zoneId);
		if (!row)
		{
			return "";
		}

		return row.boneSlot;
	}

	static string Genitive(int zoneId)
	{
		OMD_IDS_LimbZone row;

		row = FindRow(zoneId);
		if (!row)
		{
			return "";
		}

		return row.genitive;
	}

	static string Accusative(int zoneId)
	{
		OMD_IDS_LimbZone row;

		row = FindRow(zoneId);
		if (!row)
		{
			return "";
		}

		return row.accusative;
	}

	// " с" and " из" need the genitive. Everything else in the menu verb uses the accusative.
	private static int WantsGenitive(string verb)
	{
		int length;
		string tail;

		length = verb.Length();
		if (length >= 3)
		{
			tail = verb.Substring(length - 3, 3);
			if (tail == " из")
			{
				return 1;
			}
		}

		if (length >= 2)
		{
			tail = verb.Substring(length - 2, 2);
			if (tail == " с")
			{
				return 1;
			}
		}

		return 0;
	}

	// Hand pair keeps the old two lines. Variant 1 is "the other hand" only when that zone is a hand.
	// A foot is named with its own side. "на" and a bare verb take the accusative. "с" and "из" take the genitive.
	static string PairOrZone(int variantId, int zoneId, string first, string other, string verb)
	{
		string profileId;
		string limb;

		profileId = ProfileId(zoneId);
		if (profileId != "")
		{
			if (profileId != "hand")
			{
				limb = Accusative(zoneId);
				if (WantsGenitive(verb) != 0)
				{
					limb = Genitive(zoneId);
				}

				if (limb != "")
				{
					return verb + " " + limb;
				}
			}
		}

		if (variantId == 1)
		{
			return other;
		}

		return first;
	}

	// Prefix plus the genitive, or the accusative when onto is not 0. Hands return "" so the old sentence stays.
	static string Phrase(string prefix, int groupId, int onto)
	{
		int zoneId;
		string limb;

		zoneId = ZoneByHudGroup(groupId);
		if (zoneId == 0)
		{
			return "";
		}

		if (ProfileId(zoneId) == "hand")
		{
			return "";
		}

		limb = Genitive(zoneId);
		if (onto != 0)
		{
			limb = Accusative(zoneId);
		}

		if (limb == "")
		{
			return "";
		}

		return prefix + limb;
	}

	// flags[i] matches ZoneAt(i). A zero word is not ready. Variant n is the n-th ready zone.
	static void CollectFlags(int packedLeft, int packedRight, int mode, array<int> flags)
	{
		int index;
		int zoneId;
		int side;
		int packed;
		int word;
		OMD_IDS_LimbZone row;

		if (!flags)
		{
			return;
		}

		flags.Clear();
		Ensure();
		for (index = 0; index < s_Rows.Count(); index = index + 1)
		{
			row = s_Rows.Get(index);
			if (!row)
			{
				flags.Insert(0);
				continue;
			}

			zoneId = row.zoneId;
			side = row.side;
			packed = packedLeft;
			if (side != 0)
			{
				packed = packedRight;
			}

			word = FlagWord(packed, zoneId);
			if (OMD_IDS_TreatAllow.IsAllowed(zoneId, OMD_IDS_TreatAllow.TypeOfMode(mode)) == 0)
			{
				word = 0;
			}

			flags.Insert(word);
		}
	}

	static int Pick(array<int> flags, int variantId, int mode)
	{
		int index;
		int seen;
		int word;
		OMD_IDS_LimbZone row;

		if (!flags)
		{
			return 0;
		}

		if (variantId < 0)
		{
			return 0;
		}

		Ensure();
		seen = 0;
		for (index = 0; index < s_Rows.Count(); index = index + 1)
		{
			if (index >= flags.Count())
			{
				return 0;
			}

			word = flags.Get(index);
			if (OMD_IDS_HandTreatFlags.Ready(word, mode) == 0)
			{
				continue;
			}

			row = s_Rows.Get(index);
			if (!row)
			{
				continue;
			}

			if (seen == variantId)
			{
				return row.zoneId;
			}

			seen = seen + 1;
		}

		return 0;
	}

	static int ReadyCount(array<int> flags, int mode)
	{
		int index;
		int total;
		int word;

		if (!flags)
		{
			return 0;
		}

		Ensure();
		total = 0;
		for (index = 0; index < s_Rows.Count(); index = index + 1)
		{
			if (index >= flags.Count())
			{
				return total;
			}

			word = flags.Get(index);
			if (OMD_IDS_HandTreatFlags.Ready(word, mode) == 0)
			{
				continue;
			}

			total = total + 1;
		}

		return total;
	}
};

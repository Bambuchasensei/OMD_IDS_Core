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
		// Sides 2-10 are not the packed left/right words. Add() stores bitShift -1 for them.
		// TorsoSlot is the only map from these zones onto a torso int and a shift.
		Add("torso", OMD_IDS_ZoneId.UPPER_CHEST, 2, OMD_IDS_GroupId.UPPER_CHEST, "torso", "верхней части груди", "верхнюю часть груди", "chest");
		Add("torso", OMD_IDS_ZoneId.MIDDLE_CHEST, 3, OMD_IDS_GroupId.MIDDLE_CHEST, "torso", "средней части груди", "среднюю часть груди", "chest");
		Add("torso", OMD_IDS_ZoneId.PELVIS, 4, OMD_IDS_GroupId.PELVIS, "torso", "таза", "таз", "pelvis");
		Add("torso", OMD_IDS_ZoneId.LEFT_SHOULDER, 5, OMD_IDS_GroupId.LEFT_SHOULDER, "torso", "левого плеча", "левое плечо", "shoulder");
		Add("torso", OMD_IDS_ZoneId.RIGHT_SHOULDER, 6, OMD_IDS_GroupId.RIGHT_SHOULDER, "torso", "правого плеча", "правое плечо", "shoulder");
		// Sides 7-10 are meat only. No bone slot. CollectFlags clears bone and splint bits.
		Add("torso", OMD_IDS_ZoneId.LOWER_CHEST, 7, OMD_IDS_GroupId.LOWER_CHEST, "torso", "нижней части груди", "нижнюю часть груди", "");
		Add("torso", OMD_IDS_ZoneId.UPPER_ABDOMEN, 8, OMD_IDS_GroupId.UPPER_ABDOMEN, "torso", "верхней части живота", "верхнюю часть живота", "");
		Add("torso", OMD_IDS_ZoneId.LOWER_ABDOMEN, 9, OMD_IDS_GroupId.LOWER_ABDOMEN, "torso", "нижней части живота", "нижнюю часть живота", "");
		Add("torso", OMD_IDS_ZoneId.GROIN, 10, OMD_IDS_GroupId.GROIN, "torso", "паха", "пах", "");
		// Sides 11-18 are arm and leg meat. No bone slot. LimbSlot maps them. CollectFlags clears bone and splint bits.
		Add("arm", OMD_IDS_ZoneId.LEFT_UPPER_ARM, 11, OMD_IDS_GroupId.LEFT_UPPER_ARM, "arm", "верхней части левой руки", "верхнюю часть левой руки", "");
		Add("arm", OMD_IDS_ZoneId.LEFT_LOWER_ARM, 12, OMD_IDS_GroupId.LEFT_LOWER_ARM, "arm", "нижней части левой руки", "нижнюю часть левой руки", "");
		Add("arm", OMD_IDS_ZoneId.RIGHT_UPPER_ARM, 13, OMD_IDS_GroupId.RIGHT_UPPER_ARM, "arm", "верхней части правой руки", "верхнюю часть правой руки", "");
		Add("arm", OMD_IDS_ZoneId.RIGHT_LOWER_ARM, 14, OMD_IDS_GroupId.RIGHT_LOWER_ARM, "arm", "нижней части правой руки", "нижнюю часть правой руки", "");
		Add("leg", OMD_IDS_ZoneId.LEFT_UPPER_LEG, 15, OMD_IDS_GroupId.LEFT_UPPER_LEG, "leg", "верхней части левой ноги", "верхнюю часть левой ноги", "");
		Add("leg", OMD_IDS_ZoneId.LEFT_LOWER_LEG, 16, OMD_IDS_GroupId.LEFT_LOWER_LEG, "leg", "нижней части левой ноги", "нижнюю часть левой ноги", "");
		Add("leg", OMD_IDS_ZoneId.RIGHT_UPPER_LEG, 17, OMD_IDS_GroupId.RIGHT_UPPER_LEG, "leg", "верхней части правой ноги", "верхнюю часть правой ноги", "");
		Add("leg", OMD_IDS_ZoneId.RIGHT_LOWER_LEG, 18, OMD_IDS_GroupId.RIGHT_LOWER_LEG, "leg", "нижней части правой ноги", "нижнюю часть правой ноги", "");
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
		// Sides 0 and 1 fill two 16-bit slots. Any other side stays out of those ints.
		if (side != 0)
		{
			if (side != 1)
			{
				row.bitShift = -1;
			}
		}
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
		OMD_IDS_LimbZone row;

		row = FindRow(zoneId);
		if (!row)
		{
			return 0;
		}

		shift = row.bitShift;
		if (shift < 0)
		{
			return 0;
		}

		return ShiftedWord(packedSide, shift);
	}

	// Same mask as the hand ints. Bit 31 is the splint bit of the high zone, kept by the mask after the shift.
	static int ShiftedWord(int packed, int shift)
	{
		int word;

		word = packed >> shift;
		word = word & 65535;
		return word;
	}

	// which: 0 int A, 1 int B, 2 int C, 3 int D, 4 int E. -1 means this zone is not in the torso ints.
	// A shift 0 UpperChest, A shift 16 MiddleChest.
	// B shift 0 Pelvis, B shift 16 LeftShoulder.
	// C shift 0 RightShoulder, C shift 16 LowerChest.
	// D shift 0 UpperAbdomen, D shift 16 LowerAbdomen.
	// E shift 0 Groin. The high half of E stays 0.
	// Arm and leg meat is LimbSlot, not this table.
	static void TorsoSlot(int zoneId, out int which, out int shift)
	{
		which = -1;
		shift = 0;
		if (zoneId == OMD_IDS_ZoneId.UPPER_CHEST)
		{
			which = 0;
			shift = 0;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.MIDDLE_CHEST)
		{
			which = 0;
			shift = 16;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.PELVIS)
		{
			which = 1;
			shift = 0;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_SHOULDER)
		{
			which = 1;
			shift = 16;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_SHOULDER)
		{
			which = 2;
			shift = 0;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.LOWER_CHEST)
		{
			which = 2;
			shift = 16;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.UPPER_ABDOMEN)
		{
			which = 3;
			shift = 0;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.LOWER_ABDOMEN)
		{
			which = 3;
			shift = 16;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.GROIN)
		{
			which = 4;
			shift = 0;
		}
	}

	// which: 0 int A, 1 int B, 2 int C, 3 int D. -1 means this zone is not in the limb ints.
	// A shift 0 LeftUpperArm, A shift 16 LeftLowerArm.
	// B shift 0 RightUpperArm, B shift 16 RightLowerArm.
	// C shift 0 LeftUpperLeg, C shift 16 LeftLowerLeg.
	// D shift 0 RightUpperLeg, D shift 16 RightLowerLeg.
	static void LimbSlot(int zoneId, out int which, out int shift)
	{
		which = -1;
		shift = 0;
		if (zoneId == OMD_IDS_ZoneId.LEFT_UPPER_ARM)
		{
			which = 0;
			shift = 0;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_LOWER_ARM)
		{
			which = 0;
			shift = 16;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_UPPER_ARM)
		{
			which = 1;
			shift = 0;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_LOWER_ARM)
		{
			which = 1;
			shift = 16;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_UPPER_LEG)
		{
			which = 2;
			shift = 0;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_LOWER_LEG)
		{
			which = 2;
			shift = 16;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_UPPER_LEG)
		{
			which = 3;
			shift = 0;
			return;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_LOWER_LEG)
		{
			which = 3;
			shift = 16;
		}
	}

	static int ZoneWord(int packedLeft, int packedRight, int torsoA, int torsoB, int torsoC, int torsoD, int torsoE, int limbA, int limbB, int limbC, int limbD, int zoneId)
	{
		int which;
		int shift;
		int packed;
		OMD_IDS_LimbZone row;

		row = FindRow(zoneId);
		if (!row)
		{
			return 0;
		}

		if (row.bitShift >= 0)
		{
			packed = packedLeft;
			if (row.side != 0)
			{
				packed = packedRight;
			}

			return FlagWord(packed, zoneId);
		}

		TorsoSlot(zoneId, which, shift);
		if (which >= 0)
		{
			packed = torsoA;
			if (which == 1)
			{
				packed = torsoB;
			}

			if (which == 2)
			{
				packed = torsoC;
			}

			if (which == 3)
			{
				packed = torsoD;
			}

			if (which == 4)
			{
				packed = torsoE;
			}

			return ShiftedWord(packed, shift);
		}

		LimbSlot(zoneId, which, shift);
		if (which < 0)
		{
			return 0;
		}

		packed = limbA;
		if (which == 1)
		{
			packed = limbB;
		}

		if (which == 2)
		{
			packed = limbC;
		}

		if (which == 3)
		{
			packed = limbD;
		}

		return ShiftedWord(packed, shift);
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

	// Bone is bits 4-7. Splint is bit 15. Same layout as OMD_IDS_HandTreatFlags.Pack.
	private static int WithoutBoneOrSplint(int word)
	{
		int bone;
		int splint;

		bone = OMD_IDS_HandTreatFlags.Bone(word);
		if (bone != 0)
		{
			word = word - (bone * 16);
		}

		splint = OMD_IDS_HandTreatFlags.HasSplint(word);
		if (splint != 0)
		{
			word = word - 32768;
		}

		return word;
	}

	// flags[i] matches ZoneAt(i). A zero word is not ready. Variant n is the n-th ready zone.
	// Sides 0 and 1 stay in packedLeft and packedRight. Sides 2-10 use TorsoSlot inside ZoneWord.
	// Sides 11-18 use LimbSlot inside ZoneWord.
	// Sides 7-18 are meat. Bone bits 4-7 and splint bit 15 are cleared after the read.
	static void CollectFlags(int packedLeft, int packedRight, int torsoA, int torsoB, int torsoC, int torsoD, int torsoE, int limbA, int limbB, int limbC, int limbD, int mode, array<int> flags)
	{
		int index;
		int zoneId;
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
			word = ZoneWord(packedLeft, packedRight, torsoA, torsoB, torsoC, torsoD, torsoE, limbA, limbB, limbC, limbD, zoneId);
			if (row.side >= 7)
			{
				if (row.side <= 18)
				{
					word = WithoutBoneOrSplint(word);
				}
			}

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

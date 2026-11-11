// Zone name, id and HUD group. Grouping for the injury RPC comes from here, not from hud.json.
class OMD_IDS_ZoneCatalog
{
	private static ref array<string> s_Names;
	private static ref array<int> s_Groups;

	static int FromName(string zoneName)
	{
		Ensure();
		if (zoneName == "")
		{
			return OMD_IDS_ZoneId.NONE;
		}

		int index;
		for (index = 0; index < s_Names.Count(); index = index + 1)
		{
			if (s_Names.Get(index) == zoneName)
			{
				return index;
			}
		}

		return OMD_IDS_ZoneId.NONE;
	}

	static string GetName(int zoneId)
	{
		Ensure();
		if (zoneId < 0)
		{
			return "";
		}

		if (zoneId >= s_Names.Count())
		{
			return "";
		}

		return s_Names.Get(zoneId);
	}

	static bool IsKnown(int zoneId)
	{
		if (zoneId == OMD_IDS_ZoneId.NONE)
		{
			return false;
		}

		string zoneName = GetName(zoneId);
		if (zoneName == "")
		{
			return false;
		}

		return true;
	}

	static int GetGroupId(int zoneId)
	{
		Ensure();
		if (zoneId < 0)
		{
			return OMD_IDS_GroupId.NONE;
		}

		if (zoneId >= s_Groups.Count())
		{
			return OMD_IDS_GroupId.NONE;
		}

		return s_Groups.Get(zoneId);
	}

	// Silhouette for the HUD snapshot. GetGroupId is unchanged, including GroupId.BONE, both hands as HAND and both feet as FOOT.
	// Left and right hands, and left and right feet, are separate snapshot groups. Other paired parts stay one group.
	static int GetHudGroupId(int zoneId)
	{
		if (zoneId == OMD_IDS_ZoneId.LEFT_UPPER_ARM_BONE)
		{
			return OMD_IDS_GroupId.ARM;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_UPPER_ARM_BONE)
		{
			return OMD_IDS_GroupId.ARM;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_LOWER_ARM_BONE)
		{
			return OMD_IDS_GroupId.ARM;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_LOWER_ARM_BONE)
		{
			return OMD_IDS_GroupId.ARM;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_UPPER_LEG_BONE)
		{
			return OMD_IDS_GroupId.LEG;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_UPPER_LEG_BONE)
		{
			return OMD_IDS_GroupId.LEG;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_LOWER_LEG_BONE)
		{
			return OMD_IDS_GroupId.LEG;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_LOWER_LEG_BONE)
		{
			return OMD_IDS_GroupId.LEG;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_HAND)
		{
			return OMD_IDS_GroupId.LEFT_HAND;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_HAND)
		{
			return OMD_IDS_GroupId.RIGHT_HAND;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_FOOT)
		{
			return OMD_IDS_GroupId.LEFT_FOOT;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_FOOT)
		{
			return OMD_IDS_GroupId.RIGHT_FOOT;
		}

		return GetGroupId(zoneId);
	}

	private static void Ensure()
	{
		if (s_Names)
		{
			return;
		}

		s_Names = new array<string>();
		s_Groups = new array<int>();
		Add(OMD_IDS_ZoneId.NONE, "", OMD_IDS_GroupId.NONE);
		Add(OMD_IDS_ZoneId.HEAD, "Head", OMD_IDS_GroupId.HEAD);
		Add(OMD_IDS_ZoneId.NECK, "Neck", OMD_IDS_GroupId.HEAD);
		Add(OMD_IDS_ZoneId.UPPER_CHEST, "UpperChest", OMD_IDS_GroupId.TORSO);
		Add(OMD_IDS_ZoneId.MIDDLE_CHEST, "MiddleChest", OMD_IDS_GroupId.TORSO);
		Add(OMD_IDS_ZoneId.LOWER_CHEST, "LowerChest", OMD_IDS_GroupId.TORSO);
		Add(OMD_IDS_ZoneId.UPPER_ABDOMEN, "UpperAbdomen", OMD_IDS_GroupId.TORSO);
		Add(OMD_IDS_ZoneId.LOWER_ABDOMEN, "LowerAbdomen", OMD_IDS_GroupId.TORSO);
		Add(OMD_IDS_ZoneId.PELVIS, "Pelvis", OMD_IDS_GroupId.TORSO);
		Add(OMD_IDS_ZoneId.GROIN, "Groin", OMD_IDS_GroupId.TORSO);
		Add(OMD_IDS_ZoneId.LEFT_SHOULDER, "LeftShoulder", OMD_IDS_GroupId.TORSO);
		Add(OMD_IDS_ZoneId.LEFT_UPPER_ARM, "LeftUpperArm", OMD_IDS_GroupId.ARM);
		Add(OMD_IDS_ZoneId.LEFT_UPPER_ARM_BONE, "LeftUpperArmBone", OMD_IDS_GroupId.BONE);
		Add(OMD_IDS_ZoneId.LEFT_LOWER_ARM, "LeftLowerArm", OMD_IDS_GroupId.ARM);
		Add(OMD_IDS_ZoneId.LEFT_LOWER_ARM_BONE, "LeftLowerArmBone", OMD_IDS_GroupId.BONE);
		Add(OMD_IDS_ZoneId.LEFT_HAND, "LeftHand", OMD_IDS_GroupId.HAND);
		Add(OMD_IDS_ZoneId.RIGHT_SHOULDER, "RightShoulder", OMD_IDS_GroupId.TORSO);
		Add(OMD_IDS_ZoneId.RIGHT_UPPER_ARM, "RightUpperArm", OMD_IDS_GroupId.ARM);
		Add(OMD_IDS_ZoneId.RIGHT_UPPER_ARM_BONE, "RightUpperArmBone", OMD_IDS_GroupId.BONE);
		Add(OMD_IDS_ZoneId.RIGHT_LOWER_ARM, "RightLowerArm", OMD_IDS_GroupId.ARM);
		Add(OMD_IDS_ZoneId.RIGHT_LOWER_ARM_BONE, "RightLowerArmBone", OMD_IDS_GroupId.BONE);
		Add(OMD_IDS_ZoneId.RIGHT_HAND, "RightHand", OMD_IDS_GroupId.HAND);
		Add(OMD_IDS_ZoneId.LEFT_UPPER_LEG, "LeftUpperLeg", OMD_IDS_GroupId.LEG);
		Add(OMD_IDS_ZoneId.LEFT_UPPER_LEG_BONE, "LeftUpperLegBone", OMD_IDS_GroupId.BONE);
		Add(OMD_IDS_ZoneId.LEFT_LOWER_LEG, "LeftLowerLeg", OMD_IDS_GroupId.LEG);
		Add(OMD_IDS_ZoneId.LEFT_LOWER_LEG_BONE, "LeftLowerLegBone", OMD_IDS_GroupId.BONE);
		Add(OMD_IDS_ZoneId.LEFT_FOOT, "LeftFoot", OMD_IDS_GroupId.FOOT);
		Add(OMD_IDS_ZoneId.RIGHT_UPPER_LEG, "RightUpperLeg", OMD_IDS_GroupId.LEG);
		Add(OMD_IDS_ZoneId.RIGHT_UPPER_LEG_BONE, "RightUpperLegBone", OMD_IDS_GroupId.BONE);
		Add(OMD_IDS_ZoneId.RIGHT_LOWER_LEG, "RightLowerLeg", OMD_IDS_GroupId.LEG);
		Add(OMD_IDS_ZoneId.RIGHT_LOWER_LEG_BONE, "RightLowerLegBone", OMD_IDS_GroupId.BONE);
		Add(OMD_IDS_ZoneId.RIGHT_FOOT, "RightFoot", OMD_IDS_GroupId.FOOT);
		Add(OMD_IDS_ZoneId.BRAIN, "Brain", OMD_IDS_GroupId.NONE);
		Add(OMD_IDS_ZoneId.SPINE, "Spine", OMD_IDS_GroupId.NONE);
		Add(OMD_IDS_ZoneId.HEART, "Heart", OMD_IDS_GroupId.LUNGS);
		Add(OMD_IDS_ZoneId.LUNGS, "Lungs", OMD_IDS_GroupId.LUNGS);
		Add(OMD_IDS_ZoneId.LIVER, "Liver", OMD_IDS_GroupId.LUNGS);
		Add(OMD_IDS_ZoneId.KIDNEYS, "Kidneys", OMD_IDS_GroupId.LUNGS);
		Add(OMD_IDS_ZoneId.GUTS, "Guts", OMD_IDS_GroupId.LUNGS);
		if (s_Names.Count() != OMD_IDS_ZoneId.GUTS + 1)
		{
			Print("[OMD_IDS] zone catalog does not match OMD_IDS_ZoneId");
		}
	}

	private static void Add(int zoneId, string zoneName, int groupId)
	{
		if (s_Names.Count() != zoneId)
		{
			Print(string.Format("[OMD_IDS] zone catalog index drift at '%1'", zoneName));
		}

		s_Names.Insert(zoneName);
		s_Groups.Insert(groupId);
	}
};

#ifndef SERVER

class OMD_IDS_HudGroupMap
{
	static const string GROUP_NONE = "";
	static const string GROUP_HEAD = "Head";
	static const string GROUP_TORSO = "Torso";
	static const string GROUP_ARM = "Arm";
	static const string GROUP_HAND = "Hand";
	static const string GROUP_LEG = "Leg";
	static const string GROUP_FOOT = "Foot";
	static const string GROUP_BONE = "Bone";
	static const string GROUP_LUNGS = "Lungs";
	static const string GROUP_HEART = "Heart";
	static const string GROUP_LIVER = "Liver";
	static const string GROUP_KIDNEYS = "Kidneys";
	static const string GROUP_GUTS = "Guts";
	static const int ZONE_COUNT = 38;

	static const string ICON_HEAD = "OMD_IDS_Core/gui/icons/body/ids_head_64.png";
	static const string ICON_TORSO = "OMD_IDS_Core/gui/icons/body/ids_torso_64.png";
	static const string ICON_ARM = "OMD_IDS_Core/gui/icons/body/ids_arm_64.png";
	static const string ICON_HAND = "OMD_IDS_Core/gui/icons/body/ids_hand_64.png";
	static const string ICON_LEG = "OMD_IDS_Core/gui/icons/body/ids_leg_64.png";
	static const string ICON_FOOT = "OMD_IDS_Core/gui/icons/body/ids_foot_64.png";
	static const string ICON_BONE = "OMD_IDS_Core/gui/icons/organs/ids_bone_64.png";
	static const string ICON_LUNGS = "OMD_IDS_Core/gui/icons/organs/ids_lungs_64.png";

	private OMD_IDS_HudConfig m_Config;

	void Bind(OMD_IDS_HudConfig config)
	{
		m_Config = config;
	}

	static void FillDefaultZones(array<ref OMD_IDS_HudZoneLink> zones)
	{
		if (!zones)
		{
			return;
		}

		AddZone(zones, "Head", GROUP_HEAD);
		AddZone(zones, "Neck", GROUP_HEAD);
		AddZone(zones, "UpperChest", GROUP_TORSO);
		AddZone(zones, "MiddleChest", GROUP_TORSO);
		AddZone(zones, "LowerChest", GROUP_TORSO);
		AddZone(zones, "UpperAbdomen", GROUP_TORSO);
		AddZone(zones, "LowerAbdomen", GROUP_TORSO);
		AddZone(zones, "Pelvis", GROUP_TORSO);
		AddZone(zones, "Groin", GROUP_TORSO);
		AddZone(zones, "LeftShoulder", GROUP_TORSO);
		AddZone(zones, "RightShoulder", GROUP_TORSO);
		AddZone(zones, "LeftUpperArm", GROUP_ARM);
		AddZone(zones, "RightUpperArm", GROUP_ARM);
		AddZone(zones, "LeftLowerArm", GROUP_ARM);
		AddZone(zones, "RightLowerArm", GROUP_ARM);
		AddZone(zones, "LeftHand", GROUP_HAND);
		AddZone(zones, "RightHand", GROUP_HAND);
		AddZone(zones, "LeftUpperLeg", GROUP_LEG);
		AddZone(zones, "RightUpperLeg", GROUP_LEG);
		AddZone(zones, "LeftLowerLeg", GROUP_LEG);
		AddZone(zones, "RightLowerLeg", GROUP_LEG);
		AddZone(zones, "LeftFoot", GROUP_FOOT);
		AddZone(zones, "RightFoot", GROUP_FOOT);
		AddZone(zones, "LeftUpperArmBone", GROUP_BONE);
		AddZone(zones, "RightUpperArmBone", GROUP_BONE);
		AddZone(zones, "LeftLowerArmBone", GROUP_BONE);
		AddZone(zones, "RightLowerArmBone", GROUP_BONE);
		AddZone(zones, "LeftUpperLegBone", GROUP_BONE);
		AddZone(zones, "RightUpperLegBone", GROUP_BONE);
		AddZone(zones, "LeftLowerLegBone", GROUP_BONE);
		AddZone(zones, "RightLowerLegBone", GROUP_BONE);
		AddZone(zones, "Lungs", GROUP_LUNGS);
		AddZone(zones, "Heart", GROUP_HEART);
		AddZone(zones, "Liver", GROUP_LIVER);
		AddZone(zones, "Kidneys", GROUP_KIDNEYS);
		AddZone(zones, "Guts", GROUP_GUTS);
		AddZone(zones, "Brain", GROUP_NONE);
		AddZone(zones, "Spine", GROUP_NONE);
	}

	static void FillDefaultGroups(array<ref OMD_IDS_HudGroupIcon> groups)
	{
		if (!groups)
		{
			return;
		}

		AddGroup(groups, GROUP_HEAD, ICON_HEAD);
		AddGroup(groups, GROUP_TORSO, ICON_TORSO);
		AddGroup(groups, GROUP_ARM, ICON_ARM);
		AddGroup(groups, GROUP_HAND, ICON_HAND);
		AddGroup(groups, GROUP_LEG, ICON_LEG);
		AddGroup(groups, GROUP_FOOT, ICON_FOOT);
		AddGroup(groups, GROUP_BONE, ICON_BONE);
		AddGroup(groups, GROUP_LUNGS, ICON_LUNGS);
		AddGroup(groups, GROUP_HEART, "");
		AddGroup(groups, GROUP_LIVER, "");
		AddGroup(groups, GROUP_KIDNEYS, "");
		AddGroup(groups, GROUP_GUTS, "");
	}

	string GetHudGroup(string zoneName)
	{
		if (!m_Config)
		{
			Print("[OMD_IDS] warning: HUD config is not loaded");
			return GROUP_NONE;
		}

		if (!m_Config.zones)
		{
			Print("[OMD_IDS] warning: zone map is missing");
			return GROUP_NONE;
		}

		int index;
		for (index = 0; index < m_Config.zones.Count(); index = index + 1)
		{
			OMD_IDS_HudZoneLink link = m_Config.zones.Get(index);
			if (!link)
			{
				continue;
			}

			if (link.zone == zoneName)
			{
				return link.group;
			}
		}

		Print(string.Format("[OMD_IDS] warning: unknown zone '%1'", zoneName));
		return GROUP_NONE;
	}

	void PrintAll()
	{
		if (!m_Config)
		{
			Print("[OMD_IDS] zone map is not ready");
			return;
		}

		if (!m_Config.zones)
		{
			Print("[OMD_IDS] zone map is missing");
			return;
		}

		int index;
		for (index = 0; index < m_Config.zones.Count(); index = index + 1)
		{
			OMD_IDS_HudZoneLink link = m_Config.zones.Get(index);
			if (!link)
			{
				continue;
			}

			string groupName = link.group;
			if (groupName == GROUP_NONE)
			{
				groupName = "нет в HUD";
			}

			Print(string.Format("[OMD_IDS] %1 -> %2", link.zone, groupName));
		}

		int count = m_Config.zones.Count();
		Print(string.Format("[OMD_IDS] zones: %1", count));
		if (count != ZONE_COUNT)
		{
			Print(string.Format("[OMD_IDS] warning: expected %1 zones", ZONE_COUNT));
		}
	}

	void WarnIfZoneCountWrong()
	{
		if (!m_Config)
		{
			return;
		}

		if (!m_Config.zones)
		{
			Print("[OMD_IDS] warning: zone map is missing");
			return;
		}

		if (m_Config.zones.Count() != ZONE_COUNT)
		{
			Print(string.Format("[OMD_IDS] warning: hud.json has %1 zones, expected %2", m_Config.zones.Count(), ZONE_COUNT));
		}
	}

	private static void AddZone(array<ref OMD_IDS_HudZoneLink> zones, string zoneName, string groupId)
	{
		OMD_IDS_HudZoneLink link = new OMD_IDS_HudZoneLink();
		link.zone = zoneName;
		link.group = groupId;
		zones.Insert(link);
	}

	private static void AddGroup(array<ref OMD_IDS_HudGroupIcon> groups, string groupId, string iconPath)
	{
		OMD_IDS_HudGroupIcon groupIcon = new OMD_IDS_HudGroupIcon();
		groupIcon.id = groupId;
		groupIcon.icon = iconPath;
		groups.Insert(groupIcon);
	}
};

#endif

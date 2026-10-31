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

	// Atlas ids_icons: gui/imagesets/ids_icons.imageset (registered in config.cpp -> CfgMods -> defs -> imageSets)
	static const string ICON_HEAD = "set:ids_icons image:head";
	static const string ICON_TORSO = "set:ids_icons image:torso";
	static const string ICON_ARM = "set:ids_icons image:arm";
	static const string ICON_HAND = "set:ids_icons image:hand";
	static const string ICON_LEG = "set:ids_icons image:leg";
	static const string ICON_FOOT = "set:ids_icons image:foot";
	static const string ICON_BONE = "set:ids_icons image:bone";
	static const string ICON_LUNGS = "set:ids_icons image:lungs";
	// Old hud.json files keep direct file paths with this prefix; they are replaced by atlas defaults on load
	static const string LEGACY_ICON_PREFIX = "OMD_IDS_Core/gui/icons/";

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

	static string GetDefaultIcon(string groupId)
	{
		array<ref OMD_IDS_HudGroupIcon> defaults = new array<ref OMD_IDS_HudGroupIcon>();
		FillDefaultGroups(defaults);

		int index;
		for (index = 0; index < defaults.Count(); index = index + 1)
		{
			OMD_IDS_HudGroupIcon groupIcon = defaults.Get(index);
			if (!groupIcon)
			{
				continue;
			}

			if (groupIcon.id == groupId)
			{
				return groupIcon.icon;
			}
		}

		return "";
	}

	static bool IsLegacyIcon(string iconPath)
	{
		if (iconPath == "")
		{
			return false;
		}

		return iconPath.IndexOf(LEGACY_ICON_PREFIX) == 0;
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

#ifndef SERVER

class OMD_IDS_HudConfig
{
	static const string PROFILE_DIR = "$profile:OMD_IDS";
	static const string PROFILE_FILE = "$profile:OMD_IDS/hud.json";
	static const string LAYOUT_ROOT = "OMD_IDS_Core/gui/layouts/ids_hud.layout";
	static const string LAYOUT_ICON = "OMD_IDS_Core/gui/layouts/ids_hud_icon.layout";
	static const int MIN_ICONS_PER_ROW = 1;

	bool activeEnabled;
	bool debugEnabled;
	float positionX;
	float positionY;
	float iconSize;
	float iconGap;
	float rowGap;
	int iconsPerRow;
	float labelBand;
	float labelOffsetX;
	float labelOffsetY;
	int labelTextSize;
	float counterOffsetX;
	float counterOffsetY;
	float counterWidth;
	float counterHeight;
	int counterTextSize;
	int colorA;
	int colorR;
	int colorG;
	int colorB;
	int outlineSize;
	int outlineA;
	int outlineR;
	int outlineG;
	int outlineB;
	float badgeSize;
	float badgeOffsetX;
	float badgeOffsetY;
	ref array<ref OMD_IDS_HudZoneLink> zones;
	ref array<ref OMD_IDS_HudGroupIcon> groups;

	static OMD_IDS_HudConfig LoadOrCreate()
	{
		EnsureFolder();
		OMD_IDS_HudConfig fallback;
		if (!FileExist(PROFILE_FILE))
		{
			fallback = new OMD_IDS_HudConfig();
			fallback.ApplyDefaults();
			fallback.Save();
			Print("[OMD_IDS] created $profile:OMD_IDS/hud.json");
			return fallback;
		}

		string errorMessage;
		OMD_IDS_HudConfig loaded;
		bool ok = JsonFileLoader<OMD_IDS_HudConfig>.LoadFile(PROFILE_FILE, loaded, errorMessage);
		if (!ok)
		{
			Print(string.Format("[OMD_IDS] hud.json was not loaded: %1", errorMessage));
			fallback = new OMD_IDS_HudConfig();
			fallback.ApplyDefaults();
			return fallback;
		}

		if (!loaded)
		{
			Print("[OMD_IDS] hud.json loaded empty, using defaults");
			fallback = new OMD_IDS_HudConfig();
			fallback.ApplyDefaults();
			return fallback;
		}

		bool iconsChanged = loaded.MigrateLegacyIcons();
		bool groupAdded = loaded.EnsureOrganWoundGroup();
		loaded.ApplyMissingBadgeDefaults();
		if (iconsChanged)
		{
			Print("[OMD_IDS] hud.json: icon paths switched to atlas ids_icons");
		}

		if (groupAdded)
		{
			Print("[OMD_IDS] hud.json: added OrganWound icon");
		}

		bool rewrite = iconsChanged;
		if (groupAdded)
		{
			rewrite = true;
		}

		if (rewrite)
		{
			loaded.Save();
		}

		return loaded;
	}

	bool MigrateLegacyIcons()
	{
		if (!groups)
		{
			return false;
		}

		bool changed = false;
		int index;
		for (index = 0; index < groups.Count(); index = index + 1)
		{
			OMD_IDS_HudGroupIcon groupIcon = groups.Get(index);
			if (!groupIcon)
			{
				continue;
			}

			if (!OMD_IDS_HudGroupMap.IsLegacyIcon(groupIcon.icon))
			{
				continue;
			}

			groupIcon.icon = OMD_IDS_HudGroupMap.GetDefaultIcon(groupIcon.id);
			changed = true;
		}

		return changed;
	}

	bool EnsureOrganWoundGroup()
	{
		string groupId = OMD_IDS_GroupCatalog.GetName(OMD_IDS_GroupId.ORGAN_WOUND);
		if (groupId == "")
		{
			return false;
		}

		if (!groups)
		{
			return false;
		}

		if (HasGroup(groupId))
		{
			return false;
		}

		OMD_IDS_HudGroupIcon groupIcon = new OMD_IDS_HudGroupIcon();
		groupIcon.id = groupId;
		groupIcon.icon = OMD_IDS_HudGroupMap.ICON_ORGAN_WOUND;
		groups.Insert(groupIcon);
		return true;
	}

	void ApplyDefaults()
	{
		activeEnabled = true;
		debugEnabled = false;
		positionX = 48;
		positionY = 160;
		iconSize = 64;
		iconGap = 8;
		rowGap = 10;
		iconsPerRow = 8;
		labelBand = 18;
		labelOffsetX = 0;
		labelOffsetY = 0;
		labelTextSize = 14;
		counterOffsetX = 36;
		counterOffsetY = 0;
		counterWidth = 28;
		counterHeight = 20;
		counterTextSize = 18;
		colorA = 255;
		colorR = 255;
		colorG = 255;
		colorB = 255;
		outlineSize = 1;
		outlineA = 255;
		outlineR = 0;
		outlineG = 0;
		outlineB = 0;
		ApplyBadgeDefaults();
		zones = new array<ref OMD_IDS_HudZoneLink>();
		groups = new array<ref OMD_IDS_HudGroupIcon>();
		OMD_IDS_HudGroupMap.FillDefaultZones(zones);
		OMD_IDS_HudGroupMap.FillDefaultGroups(groups);
	}

	private void ApplyBadgeDefaults()
	{
		badgeSize = 32;
		badgeOffsetX = 16;
		badgeOffsetY = 16;
	}

	void ApplyMissingBadgeDefaults()
	{
		if (badgeSize > 0)
		{
			return;
		}

		ApplyBadgeDefaults();
	}

	void Save()
	{
		EnsureFolder();
		string errorMessage;
		bool saved = JsonFileLoader<OMD_IDS_HudConfig>.SaveFile(PROFILE_FILE, this, errorMessage);
		if (!saved)
		{
			Print(string.Format("[OMD_IDS] hud.json was not saved: %1", errorMessage));
		}
	}

	bool HasGroup(string groupId)
	{
		if (!groups)
		{
			return false;
		}

		int index;
		for (index = 0; index < groups.Count(); index = index + 1)
		{
			OMD_IDS_HudGroupIcon groupIcon = groups.Get(index);
			if (!groupIcon)
			{
				continue;
			}

			if (groupIcon.id == groupId)
			{
				return true;
			}
		}

		return false;
	}

	string FindGroupId(string foldedToken)
	{
		if (!groups)
		{
			return "";
		}

		int index;
		for (index = 0; index < groups.Count(); index = index + 1)
		{
			OMD_IDS_HudGroupIcon groupIcon = groups.Get(index);
			if (!groupIcon)
			{
				continue;
			}

			string foldedId = groupIcon.id;
			foldedId.ToLower();
			if (foldedId == foldedToken)
			{
				return groupIcon.id;
			}
		}

		return "";
	}

	string FindIconPath(string groupId)
	{
		if (!groups)
		{
			return "";
		}

		int index;
		for (index = 0; index < groups.Count(); index = index + 1)
		{
			OMD_IDS_HudGroupIcon groupIcon = groups.Get(index);
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

	private static void EnsureFolder()
	{
		if (FileExist(PROFILE_DIR))
		{
			return;
		}

		MakeDirectory(PROFILE_DIR);
	}
};

#endif

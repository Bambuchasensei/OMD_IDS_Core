#ifndef SERVER

class OMD_IDS_HudConfig
{
	static const string PROFILE_DIR = "$profile:OMD_IDS";
	static const string PROFILE_FILE = "$profile:OMD_IDS/hud.json";
	static const string LAYOUT_ROOT = "OMD_IDS_Core/gui/layouts/ids_hud.layout";
	static const string LAYOUT_ICON = "OMD_IDS_Core/gui/layouts/ids_hud_icon.layout";
	static const string LAYOUT_PAIN = "OMD_IDS_Core/gui/layouts/ids_hud_pain.layout";
	static const int MIN_ICONS_PER_ROW = 1;
	static const int DEFAULT_WOUND_A = 255;
	static const int DEFAULT_WOUND_R = 220;
	static const int DEFAULT_WOUND_G = 20;
	static const int DEFAULT_WOUND_B = 20;
	static const int DEFAULT_BONE_A = 255;
	static const int DEFAULT_BONE_R = 230;
	static const int DEFAULT_BONE_G = 190;
	static const int DEFAULT_BONE_B = 20;
	// Старый дефолт: пиксели от левого верхнего угла. Если в файле ровно он, пишем новый.
	static const float LEGACY_POSITION_X = 48;
	static const float LEGACY_POSITION_Y = 160;
	// Поля остаются в json, миграция тоже. На экран не влияют: ряд собирает WrapSpacer.
	static const float DEFAULT_POSITION_X = 0.95;
	static const float DEFAULT_POSITION_Y = 0.82;
	static const float POSITION_EPSILON = 0.001;
	static const int DEFAULT_PAIN_BLINK_LEVELS = 3;
	static const float DEFAULT_PAIN_BLINK_PERIOD = 0.25;
	static const float DEFAULT_PAIN_ICON_SIZE = 35;
	static const int DEFAULT_PAIN_0_A = 255;
	static const int DEFAULT_PAIN_0_R = 255;
	static const int DEFAULT_PAIN_0_G = 255;
	static const int DEFAULT_PAIN_0_B = 255;
	static const int DEFAULT_PAIN_1_A = 255;
	static const int DEFAULT_PAIN_1_R = 255;
	static const int DEFAULT_PAIN_1_G = 255;
	static const int DEFAULT_PAIN_1_B = 255;
	static const int DEFAULT_PAIN_2_A = 255;
	static const int DEFAULT_PAIN_2_R = 255;
	static const int DEFAULT_PAIN_2_G = 216;
	static const int DEFAULT_PAIN_2_B = 74;
	static const int DEFAULT_PAIN_3_A = 255;
	static const int DEFAULT_PAIN_3_R = 255;
	static const int DEFAULT_PAIN_3_G = 154;
	static const int DEFAULT_PAIN_3_B = 46;
	static const int DEFAULT_PAIN_4_A = 255;
	static const int DEFAULT_PAIN_4_R = 255;
	static const int DEFAULT_PAIN_4_G = 59;
	static const int DEFAULT_PAIN_4_B = 48;

	bool activeEnabled;
	bool debugEnabled;
	float positionX;
	float positionY;
	float iconSize;
	float slotExtraWidth;
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
	int woundA;
	int woundR;
	int woundG;
	int woundB;
	int boneBulletA;
	int boneBulletR;
	int boneBulletG;
	int boneBulletB;
	int outlineSize;
	int outlineA;
	int outlineR;
	int outlineG;
	int outlineB;
	float badgeSize;
	float badgeOffsetX;
	float badgeOffsetY;
	float badgeShadowOffset;
	bool painIconEnabled;
	bool painIconBlink;
	int painIconBlinkLevels;
	float painIconBlinkPeriod;
	float painIconSize;
	int pain0A;
	int pain0R;
	int pain0G;
	int pain0B;
	int pain1A;
	int pain1R;
	int pain1G;
	int pain1B;
	int pain2A;
	int pain2R;
	int pain2G;
	int pain2B;
	int pain3A;
	int pain3R;
	int pain3G;
	int pain3B;
	int pain4A;
	int pain4R;
	int pain4G;
	int pain4B;
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

		string rawJson;
		string errorMessage;
		OMD_IDS_HudConfig loaded;
		rawJson = ReadProfileText();
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
		bool organMerged = loaded.MergeSharedOrganGroup();
		bool handsAdded = loaded.EnsureSplitHandGroups();
		loaded.ApplyMissingBadgeDefaults(rawJson);
		loaded.ApplyMissingColorDefaults(rawJson);
		loaded.ApplyMissingPositionDefaults(rawJson);
		loaded.ApplyMissingLayoutDefaults(rawJson);
		loaded.ApplyMissingPainDefaults(rawJson);
		bool positionMigrated = loaded.MigrateLegacyPosition();
		bool metricsMigrated = loaded.MigrateLegacyBadgeMetrics();
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

		if (organMerged)
		{
			rewrite = true;
		}

		if (handsAdded)
		{
			rewrite = true;
			Print("[OMD_IDS] hud.json: split hand icons");
		}

		if (positionMigrated)
		{
			rewrite = true;
			Print("[OMD_IDS] hud.json: position moved from the old top-left default");
		}

		if (metricsMigrated)
		{
			rewrite = true;
			Print("[OMD_IDS] hud.json: badge metrics moved to the 40px row and the counter column");
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

	bool EnsureSplitHandGroups()
	{
		bool changed;
		string leftId;
		string rightId;
		int index;
		OMD_IDS_HudZoneLink link;

		changed = false;
		leftId = OMD_IDS_GroupCatalog.GetName(OMD_IDS_GroupId.LEFT_HAND);
		rightId = OMD_IDS_GroupCatalog.GetName(OMD_IDS_GroupId.RIGHT_HAND);
		if (leftId == "")
		{
			return false;
		}

		if (rightId == "")
		{
			return false;
		}

		if (!groups)
		{
			return false;
		}

		if (AddHandGroup(leftId))
		{
			changed = true;
		}

		if (AddHandGroup(rightId))
		{
			changed = true;
		}

		if (!zones)
		{
			return changed;
		}

		for (index = 0; index < zones.Count(); index = index + 1)
		{
			link = zones.Get(index);
			if (!link)
			{
				continue;
			}

			if (link.zone == "LeftHand")
			{
				if (link.group != leftId)
				{
					link.group = leftId;
					changed = true;
				}
			}

			if (link.zone == "RightHand")
			{
				if (link.group != rightId)
				{
					link.group = rightId;
					changed = true;
				}
			}
		}

		return changed;
	}

	private bool AddHandGroup(string groupId)
	{
		OMD_IDS_HudGroupIcon groupIcon;

		if (HasGroup(groupId))
		{
			return false;
		}

		groupIcon = new OMD_IDS_HudGroupIcon();
		groupIcon.id = groupId;
		groupIcon.icon = OMD_IDS_HudGroupMap.ICON_HAND;
		groups.Insert(groupIcon);
		return true;
	}

	bool MergeSharedOrganGroup()
	{
		bool changed;
		string organId;
		int index;
		OMD_IDS_HudGroupIcon groupIcon;
		OMD_IDS_HudZoneLink link;

		changed = false;
		organId = OMD_IDS_GroupCatalog.GetName(OMD_IDS_GroupId.LUNGS);
		if (organId == "")
		{
			return false;
		}

		if (!groups)
		{
			return false;
		}

		if (!HasGroup(organId))
		{
			groupIcon = new OMD_IDS_HudGroupIcon();
			groupIcon.id = organId;
			groupIcon.icon = OMD_IDS_HudGroupMap.ICON_LUNGS;
			groups.Insert(groupIcon);
			changed = true;
		}

		if (!zones)
		{
			return changed;
		}

		for (index = 0; index < zones.Count(); index = index + 1)
		{
			link = zones.Get(index);
			if (!link)
			{
				continue;
			}

			if (!OMD_IDS_HudGroupMap.IsSharedOrganZone(link.zone))
			{
				continue;
			}

			if (link.group == organId)
			{
				continue;
			}

			link.group = organId;
			changed = true;
		}

		return changed;
	}

	void ApplyDefaults()
	{
		activeEnabled = true;
		debugEnabled = false;
		positionX = DEFAULT_POSITION_X;
		positionY = DEFAULT_POSITION_Y;
		iconSize = 40;
		slotExtraWidth = 12;
		iconGap = 8;
		rowGap = 10;
		iconsPerRow = 8;
		labelBand = 0;
		labelOffsetX = 0;
		labelOffsetY = 0;
		labelTextSize = 14;
		counterOffsetX = 44;
		counterOffsetY = 16;
		counterWidth = 12;
		counterHeight = 15;
		counterTextSize = 14;
		colorA = 255;
		colorR = 220;
		colorG = 220;
		colorB = 220;
		ApplyColorDefaults();
		outlineSize = 2;
		outlineA = 255;
		outlineR = 0;
		outlineG = 0;
		outlineB = 0;
		ApplyBadgeDefaults();
		badgeShadowOffset = 2;
		ApplyPainDefaults();
		zones = new array<ref OMD_IDS_HudZoneLink>();
		groups = new array<ref OMD_IDS_HudGroupIcon>();
		OMD_IDS_HudGroupMap.FillDefaultZones(zones);
		OMD_IDS_HudGroupMap.FillDefaultGroups(groups);
	}

	private void ApplyBadgeDefaults()
	{
		badgeSize = 24;
		badgeOffsetX = 19;
		badgeOffsetY = 18;
	}

	private void ApplyPainDefaults()
	{
		painIconEnabled = true;
		painIconBlink = true;
		painIconBlinkLevels = DEFAULT_PAIN_BLINK_LEVELS;
		painIconBlinkPeriod = DEFAULT_PAIN_BLINK_PERIOD;
		painIconSize = DEFAULT_PAIN_ICON_SIZE;
		pain0A = DEFAULT_PAIN_0_A;
		pain0R = DEFAULT_PAIN_0_R;
		pain0G = DEFAULT_PAIN_0_G;
		pain0B = DEFAULT_PAIN_0_B;
		pain1A = DEFAULT_PAIN_1_A;
		pain1R = DEFAULT_PAIN_1_R;
		pain1G = DEFAULT_PAIN_1_G;
		pain1B = DEFAULT_PAIN_1_B;
		pain2A = DEFAULT_PAIN_2_A;
		pain2R = DEFAULT_PAIN_2_R;
		pain2G = DEFAULT_PAIN_2_G;
		pain2B = DEFAULT_PAIN_2_B;
		pain3A = DEFAULT_PAIN_3_A;
		pain3R = DEFAULT_PAIN_3_R;
		pain3G = DEFAULT_PAIN_3_G;
		pain3B = DEFAULT_PAIN_3_B;
		pain4A = DEFAULT_PAIN_4_A;
		pain4R = DEFAULT_PAIN_4_R;
		pain4G = DEFAULT_PAIN_4_G;
		pain4B = DEFAULT_PAIN_4_B;
	}

	private void ApplyMissingPainDefaults(string rawJson)
	{
		if (!JsonHasKey(rawJson, "painIconEnabled"))
		{
			painIconEnabled = true;
		}

		if (!JsonHasKey(rawJson, "painIconBlink"))
		{
			painIconBlink = true;
		}

		if (!JsonHasKey(rawJson, "painIconBlinkLevels"))
		{
			painIconBlinkLevels = DEFAULT_PAIN_BLINK_LEVELS;
		}

		if (!JsonHasKey(rawJson, "painIconBlinkPeriod"))
		{
			painIconBlinkPeriod = DEFAULT_PAIN_BLINK_PERIOD;
		}

		if (!JsonHasKey(rawJson, "painIconSize"))
		{
			painIconSize = DEFAULT_PAIN_ICON_SIZE;
		}

		if (!JsonHasKey(rawJson, "pain0A"))
		{
			pain0A = DEFAULT_PAIN_0_A;
		}

		if (!JsonHasKey(rawJson, "pain0R"))
		{
			pain0R = DEFAULT_PAIN_0_R;
		}

		if (!JsonHasKey(rawJson, "pain0G"))
		{
			pain0G = DEFAULT_PAIN_0_G;
		}

		if (!JsonHasKey(rawJson, "pain0B"))
		{
			pain0B = DEFAULT_PAIN_0_B;
		}

		if (!JsonHasKey(rawJson, "pain1A"))
		{
			pain1A = DEFAULT_PAIN_1_A;
		}

		if (!JsonHasKey(rawJson, "pain1R"))
		{
			pain1R = DEFAULT_PAIN_1_R;
		}

		if (!JsonHasKey(rawJson, "pain1G"))
		{
			pain1G = DEFAULT_PAIN_1_G;
		}

		if (!JsonHasKey(rawJson, "pain1B"))
		{
			pain1B = DEFAULT_PAIN_1_B;
		}

		if (!JsonHasKey(rawJson, "pain2A"))
		{
			pain2A = DEFAULT_PAIN_2_A;
		}

		if (!JsonHasKey(rawJson, "pain2R"))
		{
			pain2R = DEFAULT_PAIN_2_R;
		}

		if (!JsonHasKey(rawJson, "pain2G"))
		{
			pain2G = DEFAULT_PAIN_2_G;
		}

		if (!JsonHasKey(rawJson, "pain2B"))
		{
			pain2B = DEFAULT_PAIN_2_B;
		}

		if (!JsonHasKey(rawJson, "pain3A"))
		{
			pain3A = DEFAULT_PAIN_3_A;
		}

		if (!JsonHasKey(rawJson, "pain3R"))
		{
			pain3R = DEFAULT_PAIN_3_R;
		}

		if (!JsonHasKey(rawJson, "pain3G"))
		{
			pain3G = DEFAULT_PAIN_3_G;
		}

		if (!JsonHasKey(rawJson, "pain3B"))
		{
			pain3B = DEFAULT_PAIN_3_B;
		}

		if (!JsonHasKey(rawJson, "pain4A"))
		{
			pain4A = DEFAULT_PAIN_4_A;
		}

		if (!JsonHasKey(rawJson, "pain4R"))
		{
			pain4R = DEFAULT_PAIN_4_R;
		}

		if (!JsonHasKey(rawJson, "pain4G"))
		{
			pain4G = DEFAULT_PAIN_4_G;
		}

		if (!JsonHasKey(rawJson, "pain4B"))
		{
			pain4B = DEFAULT_PAIN_4_B;
		}
	}

	private void ApplyColorDefaults()
	{
		woundA = DEFAULT_WOUND_A;
		woundR = DEFAULT_WOUND_R;
		woundG = DEFAULT_WOUND_G;
		woundB = DEFAULT_WOUND_B;
		boneBulletA = DEFAULT_BONE_A;
		boneBulletR = DEFAULT_BONE_R;
		boneBulletG = DEFAULT_BONE_G;
		boneBulletB = DEFAULT_BONE_B;
	}

	int MarkColor(string label)
	{
		int treatmentWhite;

		treatmentWhite = ARGB(255, 255, 255, 255);
		if (label == OMD_IDS_InjuryTypeCatalog.LABEL_SUTURED)
		{
			return treatmentWhite;
		}

		if (label == OMD_IDS_InjuryTypeCatalog.LABEL_DRESSING_CLEAN)
		{
			return treatmentWhite;
		}

		if (label == OMD_IDS_InjuryTypeCatalog.LABEL_SPLINTED)
		{
			return treatmentWhite;
		}

		if (label == OMD_IDS_InjuryTypeCatalog.LABEL_DRESSING_DIRTY)
		{
			return PainColor(2);
		}

		if (OMD_IDS_InjuryTypeCatalog.IsWoundMark(label))
		{
			return ARGB(woundA, woundR, woundG, woundB);
		}

		if (OMD_IDS_InjuryTypeCatalog.IsBoneBulletMark(label))
		{
			return ARGB(boneBulletA, boneBulletR, boneBulletG, boneBulletB);
		}

		return ARGB(colorA, colorR, colorG, colorB);
	}

	int PainColor(int level)
	{
		if (level == 0)
		{
			return ARGB(pain0A, pain0R, pain0G, pain0B);
		}

		if (level == 1)
		{
			return ARGB(pain1A, pain1R, pain1G, pain1B);
		}

		if (level == 2)
		{
			return ARGB(pain2A, pain2R, pain2G, pain2B);
		}

		if (level == 3)
		{
			return ARGB(pain3A, pain3R, pain3G, pain3B);
		}

		if (level == 4)
		{
			return ARGB(pain4A, pain4R, pain4G, pain4B);
		}

		return ARGB(255, 255, 255, 255);
	}

	bool MigrateLegacyBadgeMetrics()
	{
		bool changed;
		bool counterMigrated;

		changed = false;
		counterMigrated = false;
		if (Near(iconSize, 64))
		{
			iconSize = 35;
			changed = true;
		}

		if (Near(iconSize, 35))
		{
			iconSize = 40;
			changed = true;
		}

		if (Near(badgeSize, 32))
		{
			if (Near(badgeOffsetX, 16))
			{
				if (Near(badgeOffsetY, 16))
				{
					badgeSize = 18;
					badgeOffsetX = 17;
					badgeOffsetY = 17;
					changed = true;
				}
			}
		}

		if (Near(badgeSize, 18))
		{
			if (Near(badgeOffsetX, 17))
			{
				if (Near(badgeOffsetY, 17))
				{
					badgeSize = 24;
					badgeOffsetX = 19;
					badgeOffsetY = 18;
					changed = true;
				}
				else
				{
					badgeSize = 24;
					changed = true;
				}
			}
			else
			{
				badgeSize = 24;
				changed = true;
			}
		}

		if (Near(badgeSize, 24))
		{
			if (Near(badgeOffsetX, 19))
			{
				if (Near(badgeOffsetY, -2))
				{
					badgeOffsetY = 18;
					changed = true;
				}
			}
		}

		if (Near(counterOffsetX, 36))
		{
			if (Near(counterOffsetY, 0))
			{
				if (Near(counterWidth, 28))
				{
					if (Near(counterHeight, 20))
					{
						counterOffsetX = 17;
						counterOffsetY = 1;
						counterWidth = 18;
						counterHeight = 15;
						counterTextSize = 14;
						changed = true;
					}
				}
			}
		}

		if (Near(counterOffsetX, 17))
		{
			if (Near(counterOffsetY, 1))
			{
				if (Near(counterWidth, 18))
				{
					if (Near(counterHeight, 15))
					{
						counterOffsetX = 44;
						counterOffsetY = 16;
						counterWidth = 12;
						counterHeight = 15;
						changed = true;
						counterMigrated = true;
					}
				}
			}
		}

		if (Near(counterOffsetX, 44))
		{
			if (Near(counterOffsetY, 0))
			{
				if (Near(counterWidth, 12))
				{
					if (Near(counterHeight, 15))
					{
						counterOffsetY = 16;
						changed = true;
					}
				}
			}
		}

		if (counterMigrated)
		{
			if (outlineSize == 1)
			{
				outlineSize = 2;
				changed = true;
			}
		}

		if (colorA == 255)
		{
			if (colorR == 255)
			{
				if (colorG == 255)
				{
					if (colorB == 255)
					{
						colorR = 220;
						colorG = 220;
						colorB = 220;
						changed = true;
					}
				}
			}
		}

		return changed;
	}

	bool MigrateLegacyPosition()
	{
		if (!Near(positionX, LEGACY_POSITION_X))
		{
			return false;
		}

		if (!Near(positionY, LEGACY_POSITION_Y))
		{
			return false;
		}

		positionX = DEFAULT_POSITION_X;
		positionY = DEFAULT_POSITION_Y;
		return true;
	}

	void ApplyMissingBadgeDefaults(string rawJson)
	{
		if (JsonHasKey(rawJson, "badgeSize"))
		{
			return;
		}

		ApplyBadgeDefaults();
	}

	private void ApplyMissingColorDefaults(string rawJson)
	{
		if (!JsonHasKey(rawJson, "woundA"))
		{
			woundA = DEFAULT_WOUND_A;
		}

		if (!JsonHasKey(rawJson, "woundR"))
		{
			woundR = DEFAULT_WOUND_R;
		}

		if (!JsonHasKey(rawJson, "woundG"))
		{
			woundG = DEFAULT_WOUND_G;
		}

		if (!JsonHasKey(rawJson, "woundB"))
		{
			woundB = DEFAULT_WOUND_B;
		}

		if (!JsonHasKey(rawJson, "boneBulletA"))
		{
			boneBulletA = DEFAULT_BONE_A;
		}

		if (!JsonHasKey(rawJson, "boneBulletR"))
		{
			boneBulletR = DEFAULT_BONE_R;
		}

		if (!JsonHasKey(rawJson, "boneBulletG"))
		{
			boneBulletG = DEFAULT_BONE_G;
		}

		if (!JsonHasKey(rawJson, "boneBulletB"))
		{
			boneBulletB = DEFAULT_BONE_B;
		}
	}

	private void ApplyMissingPositionDefaults(string rawJson)
	{
		if (!JsonHasKey(rawJson, "positionX"))
		{
			positionX = DEFAULT_POSITION_X;
		}

		if (!JsonHasKey(rawJson, "positionY"))
		{
			positionY = DEFAULT_POSITION_Y;
		}
	}

	private void ApplyMissingLayoutDefaults(string rawJson)
	{
		if (!JsonHasKey(rawJson, "slotExtraWidth"))
		{
			slotExtraWidth = 12;
		}

		if (!JsonHasKey(rawJson, "badgeShadowOffset"))
		{
			badgeShadowOffset = 2;
		}
	}

	private static bool Near(float value, float expected)
	{
		float delta;

		delta = value - expected;
		if (delta < 0)
		{
			delta = -delta;
		}

		if (delta <= POSITION_EPSILON)
		{
			return true;
		}

		return false;
	}

	private static bool JsonHasKey(string json, string key)
	{
		string token;
		int at;

		if (json == "")
		{
			return false;
		}

		if (key == "")
		{
			return false;
		}

		token = "\"" + key + "\"";
		at = json.IndexOf(token);
		if (at < 0)
		{
			return false;
		}

		return true;
	}

	private static string ReadProfileText()
	{
		FileHandle handle;
		string content;

		content = "";
		if (!FileExist(PROFILE_FILE))
		{
			return content;
		}

		handle = OpenFile(PROFILE_FILE, FileMode.READ);
		if (handle == 0)
		{
			return content;
		}

		ReadFile(handle, content, 1000000);
		CloseFile(handle);
		return content;
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

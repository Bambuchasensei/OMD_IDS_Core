#ifndef SERVER

class OMD_IDS_HudIcon
{
	private static ref array<string> s_ReportedPaths;

	private Widget m_Root;
	private Widget m_Plate;
	private ImageWidget m_Image;
	private ImageWidget m_BadgeShadow;
	private ImageWidget m_Badge;
	private TextWidget m_Counter;

	void Create()
	{
		if (!GetGame())
		{
			return;
		}

		if (!GetGame().GetWorkspace())
		{
			return;
		}

		m_Root = GetGame().GetWorkspace().CreateWidgets(OMD_IDS_HudConfig.LAYOUT_ICON);
		if (!m_Root)
		{
			Print("[OMD_IDS] failed to create ids_hud_icon.layout");
			return;
		}

		m_Plate = m_Root.FindAnyWidget("IdsHudIconPlate");
		ImageWidget image;
		ImageWidget badgeShadow;
		ImageWidget badge;
		TextWidget counter;
		if (Class.CastTo(image, m_Root.FindAnyWidget("IdsHudIconImage")))
		{
			m_Image = image;
		}

		if (Class.CastTo(badgeShadow, m_Root.FindAnyWidget("IdsHudIconBadgeShadow")))
		{
			m_BadgeShadow = badgeShadow;
		}

		if (Class.CastTo(badge, m_Root.FindAnyWidget("IdsHudIconBadge")))
		{
			m_Badge = badge;
		}

		if (Class.CastTo(counter, m_Root.FindAnyWidget("IdsHudIconCounter")))
		{
			m_Counter = counter;
		}

		if (!m_Image)
		{
			WarnOnce("IdsHudIconImage", "[OMD_IDS] icon layout is missing IdsHudIconImage");
		}

		if (!m_Badge)
		{
			WarnOnce("IdsHudIconBadge", "[OMD_IDS] icon layout is missing IdsHudIconBadge");
		}
	}

	Widget GetRoot()
	{
		return m_Root;
	}

	void Apply(OMD_IDS_HudDamageEntry entry, OMD_IDS_HudConfig config)
	{
		int groupColor;
		int markColor;
		float slotWidth;

		if (!m_Root)
		{
			return;
		}

		if (!entry)
		{
			return;
		}

		if (!config)
		{
			return;
		}

		slotWidth = config.iconSize + config.slotExtraWidth;
		m_Root.SetSize(slotWidth, config.iconSize);
		groupColor = ARGB(config.colorA, config.colorR, config.colorG, config.colorB);
		markColor = config.MarkColor(entry.label);
		ApplyPlate(config, groupColor);
		ApplyImage(entry, config, groupColor);
		ApplyBadge(entry, config, markColor);
		ApplyCounter(entry, config);
	}

	void Destroy()
	{
		if (m_Root)
		{
			m_Root.Unlink();
		}

		m_Root = null;
		m_Plate = null;
		m_Image = null;
		m_BadgeShadow = null;
		m_Badge = null;
		m_Counter = null;
	}

	static void ResetWarnings()
	{
		s_ReportedPaths = null;
	}

	private void ApplyPlate(OMD_IDS_HudConfig config, int color)
	{
		if (!m_Plate)
		{
			return;
		}

		m_Plate.SetPos(0, 0);
		m_Plate.SetSize(config.iconSize, config.iconSize);
		m_Plate.SetColor(color);
		m_Plate.Show(false);
	}

	private void ApplyImage(OMD_IDS_HudDamageEntry entry, OMD_IDS_HudConfig config, int color)
	{
		string imagePath;
		bool loaded;

		if (!m_Image)
		{
			return;
		}

		m_Image.SetPos(0, 0);
		m_Image.SetSize(config.iconSize, config.iconSize);
		m_Image.SetColor(color);
		imagePath = config.FindIconPath(entry.groupId);
		if (imagePath == "")
		{
			m_Image.Show(false);
			ShowPlate();
			LogShow(entry, imagePath);
			WarnOnce(entry.groupId, string.Format("[OMD_IDS] no icon file for group '%1'", entry.groupId));
			return;
		}

		loaded = m_Image.LoadImageFile(0, imagePath, false);
		if (!loaded)
		{
			m_Image.Show(false);
			ShowPlate();
			LogShow(entry, imagePath);
			WarnOnce(imagePath, string.Format("[OMD_IDS] icon did not load: %1", imagePath));
			return;
		}

		if (!m_Image.SetImage(0))
		{
			m_Image.Show(false);
			ShowPlate();
			LogShow(entry, imagePath);
			WarnOnce(imagePath, string.Format("[OMD_IDS] icon did not load: %1", imagePath));
			return;
		}

		m_Image.Show(true);
		LogShow(entry, imagePath);
		if (m_Plate)
		{
			m_Plate.Show(false);
		}
	}

	private void ApplyBadge(OMD_IDS_HudDamageEntry entry, OMD_IDS_HudConfig config, int color)
	{
		float size;
		float shadowX;
		float shadowY;
		string badgePath;
		bool loaded;

		if (m_Badge)
		{
			m_Badge.Show(false);
		}

		if (m_BadgeShadow)
		{
			m_BadgeShadow.Show(false);
		}

		if (!m_Badge)
		{
			return;
		}

		if (entry.label == "")
		{
			return;
		}

		if (!OMD_IDS_InjuryTypeCatalog.IsAtlasBadge(entry.label))
		{
			return;
		}

		size = config.badgeSize;
		if (size <= 0)
		{
			size = 24;
		}

		shadowX = config.badgeOffsetX + config.badgeShadowOffset;
		shadowY = config.badgeOffsetY + config.badgeShadowOffset;
		m_Badge.SetPos(config.badgeOffsetX, config.badgeOffsetY);
		m_Badge.SetSize(size, size);
		m_Badge.SetColor(color);
		badgePath = OMD_IDS_InjuryTypeCatalog.BadgePath(entry.label);
		loaded = m_Badge.LoadImageFile(0, badgePath, false);
		if (!loaded)
		{
			m_Badge.Show(false);
			WarnOnce(badgePath, string.Format("[OMD_IDS] wound badge did not load: %1", badgePath));
			return;
		}

		if (!m_Badge.SetImage(0))
		{
			m_Badge.Show(false);
			WarnOnce(badgePath, string.Format("[OMD_IDS] wound badge did not load: %1", badgePath));
			return;
		}

		m_Badge.Show(true);
		if (!m_BadgeShadow)
		{
			return;
		}

		m_BadgeShadow.SetPos(shadowX, shadowY);
		m_BadgeShadow.SetSize(size, size);
		m_BadgeShadow.SetColor(ARGB(255, 0, 0, 0));
		loaded = m_BadgeShadow.LoadImageFile(0, badgePath, false);
		if (!loaded)
		{
			m_BadgeShadow.Show(false);
			return;
		}

		if (!m_BadgeShadow.SetImage(0))
		{
			m_BadgeShadow.Show(false);
			return;
		}

		m_BadgeShadow.Show(true);
	}

	private void ApplyCounter(OMD_IDS_HudDamageEntry entry, OMD_IDS_HudConfig config)
	{
		int counterColor;
		int outlineColor;
		string counterText;

		if (!m_Counter)
		{
			return;
		}

		counterColor = ARGB(255, 255, 255, 255);
		outlineColor = ARGB(config.outlineA, config.outlineR, config.outlineG, config.outlineB);
		m_Counter.SetPos(config.counterOffsetX, config.counterOffsetY);
		m_Counter.SetSize(config.counterWidth, config.counterHeight);
		m_Counter.SetColor(counterColor);
		m_Counter.SetOutline(config.outlineSize, outlineColor);
		m_Counter.SetTextExactSize(config.counterTextSize);
		if (entry.count <= 1)
		{
			m_Counter.SetText("");
			m_Counter.Show(false);
			return;
		}

		counterText = string.Format("%1", entry.count);
		m_Counter.Show(true);
		m_Counter.SetText(counterText);
	}

	private void LogShow(OMD_IDS_HudDamageEntry entry, string imagePath)
	{
		if (!entry)
		{
			return;
		}

		Print(string.Format("[OMD_IDS] hud show group=%1 icon=%2", entry.groupId, imagePath));
	}

	private void ShowPlate()
	{
		if (!m_Plate)
		{
			return;
		}

		m_Plate.Show(true);
	}

	private static void WarnOnce(string key, string message)
	{
		int index;

		if (!s_ReportedPaths)
		{
			s_ReportedPaths = new array<string>();
		}

		for (index = 0; index < s_ReportedPaths.Count(); index = index + 1)
		{
			if (s_ReportedPaths.Get(index) == key)
			{
				return;
			}
		}

		s_ReportedPaths.Insert(key);
		Print(message);
	}
};

#endif

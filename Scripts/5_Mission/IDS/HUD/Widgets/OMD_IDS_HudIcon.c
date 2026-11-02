#ifndef SERVER

class OMD_IDS_HudIcon
{
	private static ref array<string> s_ReportedPaths;

	private Widget m_Root;
	private Widget m_Plate;
	private ImageWidget m_Image;
	private ImageWidget m_Badge;
	private TextWidget m_Label;
	private TextWidget m_Counter;

	void Create(Widget parent)
	{
		if (!parent)
		{
			return;
		}

		if (!GetGame())
		{
			return;
		}

		if (!GetGame().GetWorkspace())
		{
			return;
		}

		m_Root = GetGame().GetWorkspace().CreateWidgets(OMD_IDS_HudConfig.LAYOUT_ICON, parent);
		if (!m_Root)
		{
			Print("[OMD_IDS] failed to create ids_hud_icon.layout");
			return;
		}

		m_Plate = m_Root.FindWidget("IdsHudIconPlate");
		ImageWidget image;
		ImageWidget badge;
		TextWidget label;
		TextWidget counter;
		if (Class.CastTo(image, m_Root.FindAnyWidget("IdsHudIconImage")))
		{
			m_Image = image;
		}

		if (Class.CastTo(badge, m_Root.FindAnyWidget("IdsHudIconBadge")))
		{
			m_Badge = badge;
		}

		if (Class.CastTo(label, m_Root.FindAnyWidget("IdsHudIconLabel")))
		{
			m_Label = label;
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

	void Apply(OMD_IDS_HudDamageEntry entry, OMD_IDS_HudConfig config, float posX, float posY)
	{
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

		m_Root.SetPos(posX, posY);
		m_Root.SetSize(config.iconSize, config.iconSize + config.labelBand);
		int color = ARGB(config.colorA, config.colorR, config.colorG, config.colorB);
		int outlineColor = ARGB(config.outlineA, config.outlineR, config.outlineG, config.outlineB);
		ApplyPlate(config, color);
		ApplyImage(entry, config, color);
		ApplyLabel(entry, config, color, outlineColor);
		ApplyCounter(entry, config, color, outlineColor);
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
		m_Badge = null;
		m_Label = null;
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
		if (!m_Image)
		{
			return;
		}

		m_Image.SetPos(0, 0);
		m_Image.SetSize(config.iconSize, config.iconSize);
		m_Image.SetColor(color);
		string imagePath = config.FindIconPath(entry.groupId);
		if (imagePath == "")
		{
			m_Image.Show(false);
			ShowPlate();
			LogShow(entry, imagePath);
			WarnOnce(entry.groupId, string.Format("[OMD_IDS] no icon file for group '%1'", entry.groupId));
			return;
		}

		bool loaded = m_Image.LoadImageFile(0, imagePath, false);
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

	private void ApplyLabel(OMD_IDS_HudDamageEntry entry, OMD_IDS_HudConfig config, int color, int outlineColor)
	{
		if (!m_Label)
		{
			return;
		}

		float labelY = config.iconSize + config.labelOffsetY;
		m_Label.SetPos(config.labelOffsetX, labelY);
		m_Label.SetSize(config.iconSize, config.labelBand);
		m_Label.SetColor(color);
		m_Label.SetOutline(config.outlineSize, outlineColor);
		m_Label.SetTextExactSize(config.labelTextSize);
		if (!m_Badge)
		{
			m_Label.SetText(entry.label);
			return;
		}

		m_Badge.Show(false);
		if (entry.label == "")
		{
			m_Label.SetText("");
			return;
		}

		if (!OMD_IDS_InjuryTypeCatalog.IsAtlasBadge(entry.label))
		{
			m_Label.SetText(entry.label);
			return;
		}

		m_Label.SetText("");
		float size = config.badgeSize;
		if (size <= 0)
		{
			size = 32;
		}

		m_Badge.SetPos(config.badgeOffsetX, config.badgeOffsetY);
		m_Badge.SetSize(size, size);
		m_Badge.SetColor(color);
		string badgePath = OMD_IDS_InjuryTypeCatalog.BadgePath(entry.label);
		bool loaded = m_Badge.LoadImageFile(0, badgePath, false);
		if (!loaded)
		{
			m_Badge.Show(false);
			m_Label.SetText(entry.label);
			WarnOnce(badgePath, string.Format("[OMD_IDS] wound badge did not load: %1", badgePath));
			return;
		}

		if (!m_Badge.SetImage(0))
		{
			m_Badge.Show(false);
			m_Label.SetText(entry.label);
			WarnOnce(badgePath, string.Format("[OMD_IDS] wound badge did not load: %1", badgePath));
			return;
		}

		m_Badge.Show(true);
	}

	private void ApplyCounter(OMD_IDS_HudDamageEntry entry, OMD_IDS_HudConfig config, int color, int outlineColor)
	{
		if (!m_Counter)
		{
			return;
		}

		m_Counter.SetPos(config.counterOffsetX, config.counterOffsetY);
		m_Counter.SetSize(config.counterWidth, config.counterHeight);
		m_Counter.SetColor(color);
		m_Counter.SetOutline(config.outlineSize, outlineColor);
		m_Counter.SetTextExactSize(config.counterTextSize);
		string counterText = "";
		if (entry.count > 1)
		{
			counterText = string.Format("x%1", entry.count);
		}

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
		if (!s_ReportedPaths)
		{
			s_ReportedPaths = new array<string>();
		}

		int index;
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

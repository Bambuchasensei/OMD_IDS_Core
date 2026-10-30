#ifndef SERVER

class OMD_IDS_HudIcon
{
	private static ref array<string> s_ReportedPaths;

	private Widget m_Root;
	private Widget m_Plate;
	private ImageWidget m_Image;
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
		TextWidget label;
		TextWidget counter;
		if (Class.CastTo(image, m_Root.FindWidget("IdsHudIconImage")))
		{
			m_Image = image;
		}

		if (Class.CastTo(label, m_Root.FindWidget("IdsHudIconLabel")))
		{
			m_Label = label;
		}

		if (Class.CastTo(counter, m_Root.FindWidget("IdsHudIconCounter")))
		{
			m_Counter = counter;
		}

		if (!m_Image)
		{
			WarnOnce("IdsHudIconImage", "[OMD_IDS] icon layout is missing IdsHudIconImage");
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
			WarnOnce(entry.groupId, string.Format("[OMD_IDS] no icon file for group '%1'", entry.groupId));
			return;
		}

		bool loaded = m_Image.LoadImageFile(0, imagePath, false);
		if (!loaded)
		{
			m_Image.Show(false);
			ShowPlate();
			WarnOnce(imagePath, string.Format("[OMD_IDS] icon did not load: %1", imagePath));
			return;
		}

		m_Image.Show(true);
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
		m_Label.SetText(entry.label);
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

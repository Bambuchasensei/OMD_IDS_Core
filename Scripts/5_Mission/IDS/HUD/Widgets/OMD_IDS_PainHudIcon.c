#ifndef SERVER

// Отдельная иконка боли тела. Не входит в ряд ран.
// Один раз создаётся в ванильном NotifiersPanel, слот левее жажды.
// Детей ряда не двигаем и экранную позицию не подгоняем.
class OMD_IDS_PainHudIcon
{
	private static const string ICON_LV0 = "set:ids_icons image:pain_doom_lv0";
	private static const string ICON_LV1 = "set:ids_icons image:pain_doom_lv1";
	private static const string ICON_LV2 = "set:ids_icons image:pain_doom_lv2";
	private static const string ICON_LV3 = "set:ids_icons image:pain_doom_lv3";
	private static const string ICON_LV4 = "set:ids_icons image:pain_doom_lv4";
	private static const float DEFAULT_BLINK_PERIOD = 0.25;
	private static const float LAYOUT_ICON_SIZE = 35;

	private Widget m_Root;
	private ImageWidget m_Image;
	private int m_Level;
	private int m_FailedLevel;
	private int m_ReportedLevel;
	private int m_ShownColor;
	private float m_AppliedSize;
	private float m_BlinkTime;
	private bool m_BlinkWhite;
	private bool m_ColorSet;
	private bool m_CreateFailed;

	void OMD_IDS_PainHudIcon()
	{
		m_Level = -1;
		m_FailedLevel = -1;
		m_ReportedLevel = -1;
	}

	void Update(int level, float timeslice, OMD_IDS_HudConfig config, Widget notifiers)
	{
		int levelColor;

		if (!config)
		{
			m_FailedLevel = -1;
			Hide();
			return;
		}

		if (!config.activeEnabled)
		{
			m_FailedLevel = -1;
			Hide();
			return;
		}

		if (!config.painIconEnabled)
		{
			m_FailedLevel = -1;
			Hide();
			return;
		}

		if (level < 0)
		{
			m_FailedLevel = -1;
			Hide();
			return;
		}

		if (level > 4)
		{
			m_FailedLevel = -1;
			Hide();
			return;
		}

		if (config.painIconSize <= 0)
		{
			m_FailedLevel = -1;
			Hide();
			return;
		}

		if (m_FailedLevel == level)
		{
			return;
		}

		if (!EnsureRoot(notifiers))
		{
			return;
		}

		if (m_Level != level)
		{
			m_Level = level;
			m_BlinkTime = 0;
			m_BlinkWhite = false;
			m_ColorSet = false;
			if (!LoadFace(level))
			{
				m_FailedLevel = level;
				if (m_ReportedLevel != level)
				{
					m_ReportedLevel = level;
					Print(string.Format("[OMD_IDS] pain icon did not load: %1", IconPath(level)));
				}

				Hide();
				return;
			}

			m_FailedLevel = -1;
			Print(string.Format("[OMD_IDS] pain hud level=%1 icon=%2", level, IconPath(level)));
		}

		ApplySize(config);
		levelColor = config.PainColor(level);
		ApplyBlink(config, levelColor, timeslice);
		if (!m_Root)
		{
			return;
		}

		m_Root.Show(true);
	}

	void Destroy()
	{
		if (m_Root)
		{
			m_Root.Unlink();
		}

		m_Root = null;
		m_Image = null;
		m_Level = -1;
		m_FailedLevel = -1;
	}

	private bool EnsureRoot(Widget notifiers)
	{
		ImageWidget image;

		if (m_Root)
		{
			if (m_Image)
			{
				return true;
			}

			return false;
		}

		if (m_CreateFailed)
		{
			return false;
		}

		if (!notifiers)
		{
			return false;
		}

		if (!GetGame())
		{
			return false;
		}

		if (!GetGame().GetWorkspace())
		{
			return false;
		}

		m_Root = GetGame().GetWorkspace().CreateWidgets(OMD_IDS_HudConfig.LAYOUT_PAIN, notifiers);
		if (!m_Root)
		{
			m_CreateFailed = true;
			Print("[OMD_IDS] failed to create ids_hud_pain.layout");
			return false;
		}

		if (Class.CastTo(image, m_Root.FindAnyWidget("IdsHudPainImage")))
		{
			m_Image = image;
		}

		if (!m_Image)
		{
			m_CreateFailed = true;
			Print("[OMD_IDS] pain layout is missing IdsHudPainImage");
			m_Root.Unlink();
			m_Root = null;
			return false;
		}

		m_Root.Show(false);
		return true;
	}

	private bool LoadFace(int level)
	{
		string imagePath;
		bool loaded;

		if (!m_Image)
		{
			return false;
		}

		imagePath = IconPath(level);
		if (imagePath == "")
		{
			return false;
		}

		loaded = m_Image.LoadImageFile(0, imagePath, false);
		if (!loaded)
		{
			m_Image.Show(false);
			return false;
		}

		if (!m_Image.SetImage(0))
		{
			m_Image.Show(false);
			return false;
		}

		m_Image.Show(true);
		return true;
	}

	private void ApplySize(OMD_IDS_HudConfig config)
	{
		if (!m_Root)
		{
			return;
		}

		if (!m_Image)
		{
			return;
		}

		if (!config)
		{
			return;
		}

		if (m_AppliedSize == config.painIconSize)
		{
			return;
		}

		m_AppliedSize = config.painIconSize;
		if (config.painIconSize == LAYOUT_ICON_SIZE)
		{
			return;
		}

		m_Root.SetSize(config.painIconSize, config.painIconSize);
		m_Image.SetSize(config.painIconSize, config.painIconSize);
	}

	private void ApplyBlink(OMD_IDS_HudConfig config, int levelColor, float timeslice)
	{
		float period;
		int shown;
		bool blink;

		blink = false;
		if (config.painIconBlink)
		{
			if (m_Level >= config.painIconBlinkLevels)
			{
				blink = true;
			}
		}

		if (!blink)
		{
			m_BlinkTime = 0;
			m_BlinkWhite = false;
			Paint(levelColor);
			return;
		}

		period = config.painIconBlinkPeriod;
		if (period <= 0)
		{
			period = DEFAULT_BLINK_PERIOD;
		}

		if (m_BlinkTime > period)
		{
			m_BlinkTime = 0;
			if (m_BlinkWhite)
			{
				m_BlinkWhite = false;
			}
			else
			{
				m_BlinkWhite = true;
			}
		}
		else
		{
			m_BlinkTime = m_BlinkTime + timeslice;
		}

		shown = levelColor;
		if (m_BlinkWhite)
		{
			shown = ARGB(255, 255, 255, 255);
		}

		Paint(shown);
	}

	private void Paint(int color)
	{
		if (!m_Image)
		{
			return;
		}

		if (m_ColorSet)
		{
			if (m_ShownColor == color)
			{
				return;
			}
		}

		m_Image.SetColor(color);
		m_ShownColor = color;
		m_ColorSet = true;
	}

	private void Hide()
	{
		m_Level = -1;
		m_BlinkTime = 0;
		m_BlinkWhite = false;
		m_ColorSet = false;
		if (!m_Root)
		{
			return;
		}

		m_Root.Show(false);
	}

	private string IconPath(int level)
	{
		if (level == 0)
		{
			return ICON_LV0;
		}

		if (level == 1)
		{
			return ICON_LV1;
		}

		if (level == 2)
		{
			return ICON_LV2;
		}

		if (level == 3)
		{
			return ICON_LV3;
		}

		if (level == 4)
		{
			return ICON_LV4;
		}

		return "";
	}
};

#endif

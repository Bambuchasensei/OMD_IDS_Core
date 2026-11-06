#ifndef SERVER

// Отдельная иконка боли тела. Не входит в ряд ран и не имеет groupId.
// Корень создаётся один раз в HudPanel. Детей BadgesPanel не двигаем.
class OMD_IDS_PainHudIcon
{
	private static const string ICON_LV1 = "set:ids_icons image:pain_doom_lv1";
	private static const string ICON_LV2 = "set:ids_icons image:pain_doom_lv2";
	private static const string ICON_LV3 = "set:ids_icons image:pain_doom_lv3";
	private static const string ICON_LV4 = "set:ids_icons image:pain_doom_lv4";
	private static const float DEFAULT_BLINK_PERIOD = 0.25;
	private static const float FACE_GAP = 2;

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

	void Update(int level, float timeslice, OMD_IDS_HudConfig config, Widget parent, Widget badges)
	{
		int levelColor;

		if (!config)
		{
			m_FailedLevel = 0;
			Hide();
			return;
		}

		if (!config.activeEnabled)
		{
			m_FailedLevel = 0;
			Hide();
			return;
		}

		if (!config.painIconEnabled)
		{
			m_FailedLevel = 0;
			Hide();
			return;
		}

		if (level < 1)
		{
			m_FailedLevel = 0;
			Hide();
			return;
		}

		if (level > 4)
		{
			m_FailedLevel = 0;
			Hide();
			return;
		}

		if (config.iconSize <= 0)
		{
			m_FailedLevel = 0;
			Hide();
			return;
		}

		if (m_FailedLevel == level)
		{
			return;
		}

		if (!EnsureRoot(parent))
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

			m_FailedLevel = 0;
			Print(string.Format("[OMD_IDS] pain hud level=%1 icon=%2", level, IconPath(level)));
		}

		ApplySize(config);
		levelColor = config.PainColor(level);
		ApplyBlink(config, levelColor, timeslice);
		AlignLeft(badges, config.iconSize);
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
		m_Level = 0;
	}

	private bool EnsureRoot(Widget parent)
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

		if (!parent)
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

		m_Root = GetGame().GetWorkspace().CreateWidgets(OMD_IDS_HudConfig.LAYOUT_PAIN, parent);
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

	private void AlignLeft(Widget badges, float size)
	{
		Widget child;
		Widget leftmost;
		float childX;
		float childY;
		float childW;
		float childH;
		float bestX;
		float bestY;
		float bestH;
		float panelX;
		float panelY;
		float panelW;
		float panelH;
		float posX;
		float posY;

		if (!m_Root)
		{
			return;
		}

		if (!badges)
		{
			return;
		}

		if (size <= 0)
		{
			return;
		}

		leftmost = null;
		bestX = 0;
		bestY = 0;
		bestH = 0;
		child = badges.GetChildren();
		while (child)
		{
			if (child != m_Root)
			{
				if (child.IsVisible())
				{
					child.GetScreenSize(childW, childH);
					if (childW > 0)
					{
						child.GetScreenPos(childX, childY);
						if (!leftmost)
						{
							leftmost = child;
							bestX = childX;
							bestY = childY;
							bestH = childH;
						}
						else
						{
							if (childX < bestX)
							{
								leftmost = child;
								bestX = childX;
								bestY = childY;
								bestH = childH;
							}
						}
					}
				}
			}

			child = child.GetSibling();
		}

		if (leftmost)
		{
			posX = bestX - size - FACE_GAP;
			posY = bestY + ((bestH - size) * 0.5);
			m_Root.SetScreenPos(posX, posY, true);
			return;
		}

		badges.GetScreenPos(panelX, panelY);
		badges.GetScreenSize(panelW, panelH);
		posX = panelX + panelW - size;
		posY = panelY + ((panelH - size) * 0.5);
		m_Root.SetScreenPos(posX, posY, true);
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

		if (m_AppliedSize == config.iconSize)
		{
			return;
		}

		m_Root.SetSize(config.iconSize, config.iconSize);
		m_Image.SetPos(0, 0);
		m_Image.SetSize(config.iconSize, config.iconSize);
		m_AppliedSize = config.iconSize;
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
		m_Level = 0;
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

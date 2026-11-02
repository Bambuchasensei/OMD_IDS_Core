#ifndef SERVER

class OMD_IDS_HudManager
{
	private static ref OMD_IDS_HudManager s_Instance;

	private ref OMD_IDS_HudConfig m_Config;
	private ref OMD_IDS_HudGroupMap m_Map;
	private ref array<ref OMD_IDS_HudDamageEntry> m_Entries;
	private ref array<ref OMD_IDS_HudIcon> m_Icons;
	private Widget m_Root;

	static void Create()
	{
		if (s_Instance)
		{
			return;
		}

		s_Instance = new OMD_IDS_HudManager();
		s_Instance.Init();
	}

	static void Destroy()
	{
		if (!s_Instance)
		{
			return;
		}

		s_Instance.Shutdown();
		s_Instance = null;
	}

	static OMD_IDS_HudManager Get()
	{
		return s_Instance;
	}

	void OnUpdate(float timeslice)
	{
		RefreshPositions();
	}

	// Единственный вход списка. Команды отладки тоже идут только сюда.
	void SetActiveDamages(array<ref OMD_IDS_HudDamageEntry> entries)
	{
		array<ref OMD_IDS_HudDamageEntry> merged = new array<ref OMD_IDS_HudDamageEntry>();
		if (entries)
		{
			int index;
			for (index = 0; index < entries.Count(); index = index + 1)
			{
				MergeEntry(merged, entries.Get(index));
			}
		}

		m_Entries = merged;
		Rebuild();
	}

	array<ref OMD_IDS_HudDamageEntry> GetActiveDamages()
	{
		if (!m_Entries)
		{
			m_Entries = new array<ref OMD_IDS_HudDamageEntry>();
		}

		return m_Entries;
	}

	OMD_IDS_HudConfig GetConfig()
	{
		return m_Config;
	}

	bool IsDebugEnabled()
	{
		if (!m_Config)
		{
			return false;
		}

		return m_Config.debugEnabled;
	}

	void ReloadConfig()
	{
		OMD_IDS_HudConfig loaded = OMD_IDS_HudConfig.LoadOrCreate();
		if (!loaded)
		{
			Print("[OMD_IDS] hud.json reload failed");
			return;
		}

		m_Config = loaded;
		if (m_Map)
		{
			m_Map.Bind(m_Config);
			m_Map.WarnIfZoneCountWrong();
		}

		SetActiveDamages(m_Entries);
		Print(string.Format("[OMD_IDS] reloaded hud.json activeEnabled=%1 debugEnabled=%2", m_Config.activeEnabled, m_Config.debugEnabled));
	}

	void PrintZones()
	{
		if (!m_Map)
		{
			Print("[OMD_IDS] zone map is not ready");
			return;
		}

		m_Map.PrintAll();
	}

	string GetHudGroup(string zoneName)
	{
		if (!m_Map)
		{
			Print("[OMD_IDS] warning: zone map is not ready");
			return OMD_IDS_HudGroupMap.GROUP_NONE;
		}

		return m_Map.GetHudGroup(zoneName);
	}

	private void Init()
	{
		m_Config = OMD_IDS_HudConfig.LoadOrCreate();
		m_Map = new OMD_IDS_HudGroupMap();
		m_Map.Bind(m_Config);
		m_Map.WarnIfZoneCountWrong();
		m_Entries = new array<ref OMD_IDS_HudDamageEntry>();
		m_Icons = new array<ref OMD_IDS_HudIcon>();
		CreateRoot();
		Rebuild();
	}

	private void Shutdown()
	{
		ClearIcons();
		if (m_Root)
		{
			m_Root.Unlink();
			m_Root = null;
		}

		OMD_IDS_HudIcon.ResetWarnings();
		m_Entries = null;
		m_Config = null;
		m_Map = null;
		m_Icons = null;
	}

	private void CreateRoot()
	{
		if (!GetGame())
		{
			return;
		}

		if (!GetGame().GetWorkspace())
		{
			return;
		}

		m_Root = GetGame().GetWorkspace().CreateWidgets(OMD_IDS_HudConfig.LAYOUT_ROOT);
		if (!m_Root)
		{
			Print("[OMD_IDS] failed to create ids_hud.layout");
		}
	}

	private void Rebuild()
	{
		ClearIcons();
		if (!m_Root)
		{
			return;
		}

		if (!m_Config)
		{
			return;
		}

		if (!m_Config.activeEnabled)
		{
			m_Root.Show(false);
			return;
		}

		m_Root.Show(true);
		if (!m_Entries)
		{
			return;
		}

		if (m_Config.iconSize <= 0)
		{
			Print("[OMD_IDS] warning: iconSize must be greater than 0");
			return;
		}

		if (m_Config.iconsPerRow < OMD_IDS_HudConfig.MIN_ICONS_PER_ROW)
		{
			Print("[OMD_IDS] warning: iconsPerRow < 1, using 1");
		}

		int index;
		if (!m_Icons)
		{
			m_Icons = new array<ref OMD_IDS_HudIcon>();
		}

		for (index = 0; index < m_Entries.Count(); index = index + 1)
		{
			OMD_IDS_HudDamageEntry entry = m_Entries.Get(index);
			if (!entry)
			{
				continue;
			}

			OMD_IDS_HudIcon icon = new OMD_IDS_HudIcon();
			icon.Create(m_Root);
			icon.Apply(entry, m_Config, 0, 0);
			m_Icons.Insert(icon);
		}

		RefreshPositions();
	}

	private void RefreshPositions()
	{
		Widget workspace;
		Widget panel;
		float screenW;
		float screenH;
		float panelX;
		float panelY;
		float panelW;
		float panelH;
		float anchorRight;
		float anchorBottom;
		float shiftX;
		float shiftY;
		float stepX;
		float stepY;
		float cellW;
		float cellH;
		float columnDistance;
		float rowDistance;
		float posX;
		float posY;
		float limitX;
		float limitY;
		int perRow;
		int index;
		int columnIndex;
		int rowIndex;
		int mode;
		bool fraction;
		bool haveScreen;
		bool havePanel;

		if (!m_Config)
		{
			return;
		}

		if (!m_Icons)
		{
			return;
		}

		if (m_Icons.Count() == 0)
		{
			return;
		}

		if (m_Config.iconSize <= 0)
		{
			return;
		}

		perRow = m_Config.iconsPerRow;
		if (perRow < OMD_IDS_HudConfig.MIN_ICONS_PER_ROW)
		{
			perRow = OMD_IDS_HudConfig.MIN_ICONS_PER_ROW;
		}

		fraction = false;
		if (OMD_IDS_HudConfig.IsFraction(m_Config.positionX))
		{
			if (OMD_IDS_HudConfig.IsFraction(m_Config.positionY))
			{
				fraction = true;
			}
		}

		screenW = 0;
		screenH = 0;
		haveScreen = false;
		workspace = null;
		if (GetGame())
		{
			workspace = GetGame().GetWorkspace();
		}

		if (workspace)
		{
			workspace.GetScreenSize(screenW, screenH);
			if (screenW > 0)
			{
				if (screenH > 0)
				{
					haveScreen = true;
				}
			}
		}

		if (fraction)
		{
			if (!haveScreen)
			{
				return;
			}
		}

		panelX = 0;
		panelY = 0;
		panelW = 0;
		panelH = 0;
		havePanel = false;
		panel = FindStatusPanel();
		if (panel)
		{
			panel.GetScreenPos(panelX, panelY);
			panel.GetScreenSize(panelW, panelH);
			if (panelW > 0)
			{
				if (panelH > 0)
				{
					havePanel = true;
				}
			}
		}

		mode = 0;
		if (fraction)
		{
			mode = 2;
			if (havePanel)
			{
				mode = 1;
			}
		}

		anchorRight = 0;
		anchorBottom = 0;
		shiftX = 0;
		shiftY = 0;
		if (mode == 1)
		{
			if (!m_Config.IsDefaultPosition())
			{
				shiftX = (m_Config.positionX - OMD_IDS_HudConfig.DEFAULT_POSITION_X) * screenW;
				shiftY = (m_Config.positionY - OMD_IDS_HudConfig.DEFAULT_POSITION_Y) * screenH;
			}

			anchorRight = panelX + panelW + shiftX;
			anchorBottom = panelY - OMD_IDS_HudConfig.PANEL_GAP + shiftY;
		}

		if (mode == 2)
		{
			anchorRight = m_Config.positionX * screenW;
			anchorBottom = m_Config.positionY * screenH;
		}

		stepX = m_Config.iconSize + m_Config.iconGap;
		stepY = m_Config.iconSize + m_Config.labelBand + m_Config.rowGap;
		cellW = m_Config.iconSize;
		cellH = m_Config.iconSize + m_Config.labelBand;
		for (index = 0; index < m_Icons.Count(); index = index + 1)
		{
			OMD_IDS_HudIcon icon = m_Icons.Get(index);
			if (!icon)
			{
				continue;
			}

			columnIndex = index % perRow;
			rowIndex = index / perRow;
			columnDistance = columnIndex;
			rowDistance = rowIndex;
			if (mode == 0)
			{
				posX = m_Config.positionX + (columnDistance * stepX);
				posY = m_Config.positionY + (rowDistance * stepY);
			}
			else
			{
				posX = anchorRight - cellW - (columnDistance * stepX);
				posY = anchorBottom - cellH - (rowDistance * stepY);
				if (posX < 0)
				{
					posX = 0;
				}

				if (posY < 0)
				{
					posY = 0;
				}

				limitX = screenW - cellW;
				limitY = screenH - cellH;
				if (limitX < 0)
				{
					limitX = 0;
				}

				if (limitY < 0)
				{
					limitY = 0;
				}

				if (posX > limitX)
				{
					posX = limitX;
				}

				if (posY > limitY)
				{
					posY = limitY;
				}
			}

			icon.SetScreenOrigin(posX, posY);
		}
	}

	// Ванильные статусы: вода, еда, температура, кровь, здоровье. Корень — HudPanel.
	private Widget FindStatusPanel()
	{
		MissionGameplay gameplay;
		Hud hud;
		IngameHud ingame;
		Widget hudPanel;

		if (!GetGame())
		{
			return null;
		}

		if (!Class.CastTo(gameplay, GetGame().GetMission()))
		{
			return null;
		}

		hud = gameplay.GetHud();
		if (!hud)
		{
			return null;
		}

		if (!Class.CastTo(ingame, hud))
		{
			return null;
		}

		hudPanel = ingame.GetHudPanelWidget();
		if (!hudPanel)
		{
			return null;
		}

		return hudPanel.FindAnyWidget("NotifiersPanel");
	}

	private void ClearIcons()
	{
		if (!m_Icons)
		{
			return;
		}

		int index;
		for (index = 0; index < m_Icons.Count(); index = index + 1)
		{
			OMD_IDS_HudIcon icon = m_Icons.Get(index);
			if (!icon)
			{
				continue;
			}

			icon.Destroy();
		}

		m_Icons.Clear();
	}

	private void MergeEntry(array<ref OMD_IDS_HudDamageEntry> merged, OMD_IDS_HudDamageEntry source)
	{
		if (!merged)
		{
			return;
		}

		if (!source)
		{
			return;
		}

		if (source.groupId == "")
		{
			return;
		}

		if (source.count < 1)
		{
			return;
		}

		if (!m_Config)
		{
			return;
		}

		if (!m_Config.HasGroup(source.groupId))
		{
			Print(string.Format("[OMD_IDS] warning: unknown HUD group '%1'", source.groupId));
			return;
		}

		int index;
		for (index = 0; index < merged.Count(); index = index + 1)
		{
			OMD_IDS_HudDamageEntry existing = merged.Get(index);
			if (!existing)
			{
				continue;
			}

			if (existing.groupId != source.groupId)
			{
				continue;
			}

			if (existing.label != source.label)
			{
				continue;
			}

			existing.count = existing.count + source.count;
			return;
		}

		merged.Insert(source.Clone());
	}
};

#endif

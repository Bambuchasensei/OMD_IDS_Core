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

		int perRow = m_Config.iconsPerRow;
		if (perRow < OMD_IDS_HudConfig.MIN_ICONS_PER_ROW)
		{
			Print("[OMD_IDS] warning: iconsPerRow < 1, using 1");
			perRow = OMD_IDS_HudConfig.MIN_ICONS_PER_ROW;
		}

		float stepX = m_Config.iconSize + m_Config.iconGap;
		float stepY = m_Config.iconSize + m_Config.labelBand + m_Config.rowGap;
		int slot = 0;
		int index;
		int columnIndex;
		int rowIndex;
		float columnDistance;
		float rowDistance;
		float posX;
		float posY;
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

			columnIndex = slot % perRow;
			rowIndex = slot / perRow;
			columnDistance = columnIndex;
			rowDistance = rowIndex;
			posX = m_Config.positionX + (columnDistance * stepX);
			posY = m_Config.positionY + (rowDistance * stepY);
			OMD_IDS_HudIcon icon = new OMD_IDS_HudIcon();
			icon.Create(m_Root);
			icon.Apply(entry, m_Config, posX, posY);
			m_Icons.Insert(icon);
			slot = slot + 1;
		}
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

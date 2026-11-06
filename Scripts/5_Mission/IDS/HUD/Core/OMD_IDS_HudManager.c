#ifndef SERVER

class OMD_IDS_HudManager
{
	private static ref OMD_IDS_HudManager s_Instance;

	private ref OMD_IDS_HudConfig m_Config;
	private ref OMD_IDS_HudGroupMap m_Map;
	private ref array<ref OMD_IDS_HudDamageEntry> m_Entries;
	private ref array<ref OMD_IDS_HudIcon> m_Icons;
	private Widget m_BadgesPanel;
	private bool m_PanelRetry;
	private bool m_ReportedPlace;
	private bool m_ReportedMissingPanel;
	private bool m_ReportedMissingNotifiers;
	private ref OMD_IDS_PainHudIcon m_PainIcon;

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

	// Иконка боли отдельно от списка ран. Уровень читается из клиентского стора каждый тик.
	// Лицо один раз создаётся в NotifiersPanel. Ряд ран и BadgesPanel не двигаем.
	void UpdatePain(float timeslice)
	{
		int level;
		OMD_IDS_PainStore store;
		Widget notifiers;

		level = 0;
		store = OMD_IDS_PainStore.Get();
		if (store)
		{
			level = store.GetBodyLevel();
		}

		if (!m_PainIcon)
		{
			m_PainIcon = new OMD_IDS_PainHudIcon();
		}

		notifiers = FindNotifiersPanel();
		m_PainIcon.Update(level, timeslice, m_Config, notifiers);
	}

	private void Init()
	{
		m_Config = OMD_IDS_HudConfig.LoadOrCreate();
		m_Map = new OMD_IDS_HudGroupMap();
		m_Map.Bind(m_Config);
		m_Map.WarnIfZoneCountWrong();
		m_Entries = new array<ref OMD_IDS_HudDamageEntry>();
		m_Icons = new array<ref OMD_IDS_HudIcon>();
		m_ReportedPlace = false;
		m_ReportedMissingPanel = false;
		m_ReportedMissingNotifiers = false;
		Rebuild();
	}

	private void Shutdown()
	{
		CancelPanelRetry();
		ClearIcons();
		if (m_PainIcon)
		{
			m_PainIcon.Destroy();
			m_PainIcon = null;
		}

		m_BadgesPanel = null;
		OMD_IDS_HudIcon.ResetWarnings();
		m_Entries = null;
		m_Config = null;
		m_Map = null;
		m_Icons = null;
	}

	private void Rebuild()
	{
		ClearIcons();
		if (!m_Config)
		{
			return;
		}

		if (!m_Config.activeEnabled)
		{
			return;
		}

		if (!m_Entries)
		{
			return;
		}

		if (m_Entries.Count() == 0)
		{
			return;
		}

		if (m_Config.iconSize <= 0)
		{
			Print("[OMD_IDS] warning: iconSize must be greater than 0");
			return;
		}

		m_BadgesPanel = FindBadgesPanel();
		if (!m_BadgesPanel)
		{
			SchedulePanelRetry();
			return;
		}

		if (!m_Icons)
		{
			m_Icons = new array<ref OMD_IDS_HudIcon>();
		}

		int index;
		for (index = 0; index < m_Entries.Count(); index = index + 1)
		{
			OMD_IDS_HudDamageEntry entry = m_Entries.Get(index);
			if (!entry)
			{
				continue;
			}

			OMD_IDS_HudIcon icon = new OMD_IDS_HudIcon();
			icon.Create();
			icon.Apply(entry, m_Config);
			m_Icons.Insert(icon);
		}

		PlaceRow();
		m_BadgesPanel.Update();
	}

	private void SchedulePanelRetry()
	{
		if (m_PanelRetry)
		{
			return;
		}

		if (!GetGame())
		{
			return;
		}

		if (!m_ReportedMissingPanel)
		{
			m_ReportedMissingPanel = true;
			Print("[OMD_IDS] BadgesPanel is not ready, retrying");
		}

		m_PanelRetry = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RetryBadgesPanel, 100, false);
	}

	private void CancelPanelRetry()
	{
		m_PanelRetry = false;
		if (!GetGame())
		{
			return;
		}

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(RetryBadgesPanel);
	}

	private void RetryBadgesPanel()
	{
		m_PanelRetry = false;
		if (!m_Config)
		{
			return;
		}

		Rebuild();
	}

	// Ванильный ряд значков. Корень — HudPanel, панель — WrapSpacer BadgesPanel.
	private Widget FindBadgesPanel()
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

		return hudPanel.FindAnyWidget("BadgesPanel");
	}

	// Ванильный ряд статусов: крест, кровь, температура, голод, жажда.
	// Если панель ещё не создана, молчим и пробуем на следующем кадре.
	// Если корень HUD уже есть, а ряда нет — одно предупреждение, без виджета.
	private Widget FindNotifiersPanel()
	{
		MissionGameplay gameplay;
		Hud hud;
		IngameHud ingame;
		Widget hudPanel;
		Widget notifiers;

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

		notifiers = hudPanel.FindAnyWidget("NotifiersPanel");
		if (notifiers)
		{
			return notifiers;
		}

		if (!m_ReportedMissingNotifiers)
		{
			m_ReportedMissingNotifiers = true;
			Print("[OMD_IDS] NotifiersPanel was not found, pain icon is not created");
		}

		return null;
	}

	// Поток WrapSpacer идёт слева направо. AddChild дописывает в конец и оказывается правее крови.
	// AddChildAfter ставит наш значок сразу перед Bleeding, поэтому ряд остаётся слева от него.
	private void PlaceRow()
	{
		WrapSpacerWidget spacer;
		Widget bleeding;
		Widget anchor;
		Widget iconRoot;
		int index;
		bool placed;

		if (!m_BadgesPanel)
		{
			return;
		}

		if (!m_Icons)
		{
			return;
		}

		if (!Class.CastTo(spacer, m_BadgesPanel))
		{
			for (index = 0; index < m_Icons.Count(); index = index + 1)
			{
				if (!m_Icons.Get(index))
				{
					continue;
				}

				iconRoot = m_Icons.Get(index).GetRoot();
				DetachWidget(iconRoot);
				if (!iconRoot)
				{
					continue;
				}

				m_BadgesPanel.AddChild(iconRoot);
			}

			return;
		}

		bleeding = m_BadgesPanel.FindAnyWidget("Bleeding");
		anchor = FindChildBefore(m_BadgesPanel, bleeding);
		placed = false;
		for (index = 0; index < m_Icons.Count(); index = index + 1)
		{
			if (!m_Icons.Get(index))
			{
				continue;
			}

			iconRoot = m_Icons.Get(index).GetRoot();
			if (!iconRoot)
			{
				continue;
			}

			DetachWidget(iconRoot);
			if (anchor)
			{
				spacer.AddChildAfter(iconRoot, anchor);
				anchor = iconRoot;
				placed = true;
				continue;
			}

			InsertAtFront(m_BadgesPanel, iconRoot);
			anchor = iconRoot;
			placed = true;
		}

		if (!placed)
		{
			return;
		}

		if (m_ReportedPlace)
		{
			return;
		}

		m_ReportedPlace = true;
		if (bleeding)
		{
			Print("[OMD_IDS] hud row uses AddChildAfter, left of Bleeding");
			return;
		}

		Print("[OMD_IDS] hud row uses InsertAtFront, Bleeding widget was missing");
	}

	private Widget FindChildBefore(Widget panel, Widget target)
	{
		Widget child;
		Widget previous;

		if (!panel)
		{
			return null;
		}

		if (!target)
		{
			return null;
		}

		previous = null;
		child = panel.GetChildren();
		while (child)
		{
			if (child == target)
			{
				return previous;
			}

			previous = child;
			child = child.GetSibling();
		}

		return null;
	}

	private void InsertAtFront(Widget panel, Widget iconRoot)
	{
		array<Widget> existing;
		Widget child;
		Widget next;
		int index;

		if (!panel)
		{
			return;
		}

		if (!iconRoot)
		{
			return;
		}

		DetachWidget(iconRoot);
		existing = new array<Widget>();
		child = panel.GetChildren();
		while (child)
		{
			next = child.GetSibling();
			existing.Insert(child);
			child = next;
		}

		for (index = 0; index < existing.Count(); index = index + 1)
		{
			panel.RemoveChild(existing.Get(index));
		}

		panel.AddChild(iconRoot);
		for (index = 0; index < existing.Count(); index = index + 1)
		{
			panel.AddChild(existing.Get(index));
		}
	}

	private void DetachWidget(Widget iconRoot)
	{
		Widget holder;

		if (!iconRoot)
		{
			return;
		}

		holder = iconRoot.GetParent();
		if (!holder)
		{
			return;
		}

		holder.RemoveChild(iconRoot);
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

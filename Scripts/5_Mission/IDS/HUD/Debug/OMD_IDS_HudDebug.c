#ifndef SERVER

class OMD_IDS_HudDebug
{
	static const string COMMAND = "ids_hud";

	static bool TryHandle(string text)
	{
		if (text == "")
		{
			return false;
		}

		string trimmed = text.Trim();
		int prefixLength = COMMAND.Length();
		if (trimmed.Length() < prefixLength)
		{
			return false;
		}

		string head = trimmed.Substring(0, prefixLength);
		head.ToLower();
		if (head != COMMAND)
		{
			return false;
		}

		if (trimmed.Length() > prefixLength)
		{
			string separator = trimmed.Substring(prefixLength, 1);
			if (separator != " ")
			{
				return false;
			}
		}

		Handle(trimmed);
		return true;
	}

	private static void Handle(string line)
	{
		OMD_IDS_HudManager manager = OMD_IDS_HudManager.Get();
		if (!manager)
		{
			Print("[OMD_IDS] HUD is not ready");
			return;
		}

		TStringArray parts = new TStringArray();
		line.Split(" ", parts);
		if (parts.Count() < 2)
		{
			PrintUsage();
			return;
		}

		string command = parts.Get(1);
		command.ToLower();
		if (command == "reload")
		{
			if (parts.Count() != 2)
			{
				PrintUsage();
				return;
			}

			manager.ReloadConfig();
			return;
		}

		if (!manager.IsDebugEnabled())
		{
			Print("[OMD_IDS] HUD debug is off. Set debugEnabled to true in $profile:OMD_IDS/hud.json, then run: ids_hud reload");
			return;
		}

		if (command == "clear")
		{
			if (parts.Count() != 2)
			{
				PrintUsage();
				return;
			}

			ClearGroups(manager);
			return;
		}

		if (command == "zones")
		{
			if (parts.Count() != 2)
			{
				PrintUsage();
				return;
			}

			manager.PrintZones();
			return;
		}

		if (command == "show")
		{
			ShowGroup(manager, parts);
			return;
		}

		if (command == "hide")
		{
			HideGroup(manager, parts);
			return;
		}

		PrintUsage();
	}

	private static void ShowGroup(OMD_IDS_HudManager manager, TStringArray parts)
	{
		if (parts.Count() < 3)
		{
			PrintUsage();
			return;
		}

		if (parts.Count() > 4)
		{
			Print("[OMD_IDS] label is one word. Example: ids_hud show Head scratch");
			return;
		}

		string token = parts.Get(2);
		string label = "";
		if (parts.Count() == 4)
		{
			label = parts.Get(3);
		}

		string groupId = ResolveGroup(manager.GetConfig(), token);
		if (groupId == "")
		{
			Print(string.Format("[OMD_IDS] unknown HUD group '%1'", token));
			return;
		}

		array<ref OMD_IDS_HudDamageEntry> current = manager.GetActiveDamages();
		array<ref OMD_IDS_HudDamageEntry> next = new array<ref OMD_IDS_HudDamageEntry>();
		bool found = false;
		int index;
		for (index = 0; index < current.Count(); index = index + 1)
		{
			OMD_IDS_HudDamageEntry entry = current.Get(index);
			if (!entry)
			{
				continue;
			}

			OMD_IDS_HudDamageEntry copy = entry.Clone();
			if (copy.groupId == groupId)
			{
				if (copy.label == label)
				{
					if (!found)
					{
						copy.count = copy.count + 1;
						found = true;
					}
				}
			}

			next.Insert(copy);
		}

		if (!found)
		{
			OMD_IDS_HudDamageEntry created = new OMD_IDS_HudDamageEntry();
			created.groupId = groupId;
			created.label = label;
			created.count = 1;
			next.Insert(created);
		}

		manager.SetActiveDamages(next);
		Print(string.Format("[OMD_IDS] show %1 label '%2'", groupId, label));
	}

	private static void HideGroup(OMD_IDS_HudManager manager, TStringArray parts)
	{
		if (parts.Count() != 3)
		{
			PrintUsage();
			return;
		}

		string token = parts.Get(2);
		string groupId = ResolveGroup(manager.GetConfig(), token);
		if (groupId == "")
		{
			Print(string.Format("[OMD_IDS] unknown HUD group '%1'", token));
			return;
		}

		array<ref OMD_IDS_HudDamageEntry> current = manager.GetActiveDamages();
		array<ref OMD_IDS_HudDamageEntry> next = new array<ref OMD_IDS_HudDamageEntry>();
		int index;
		for (index = 0; index < current.Count(); index = index + 1)
		{
			OMD_IDS_HudDamageEntry entry = current.Get(index);
			if (!entry)
			{
				continue;
			}

			if (entry.groupId == groupId)
			{
				continue;
			}

			next.Insert(entry.Clone());
		}

		manager.SetActiveDamages(next);
		Print(string.Format("[OMD_IDS] hide %1", groupId));
	}

	private static void ClearGroups(OMD_IDS_HudManager manager)
	{
		array<ref OMD_IDS_HudDamageEntry> empty = new array<ref OMD_IDS_HudDamageEntry>();
		manager.SetActiveDamages(empty);
		Print("[OMD_IDS] clear");
	}

	private static string ResolveGroup(OMD_IDS_HudConfig config, string token)
	{
		if (token == "")
		{
			return "";
		}

		string alias = Alias(token);
		if (alias != "")
		{
			return alias;
		}

		if (!config)
		{
			return "";
		}

		string folded = token;
		folded.ToLower();
		return config.FindGroupId(folded);
	}

	private static string Alias(string token)
	{
		if (token == "Голова")
		{
			return OMD_IDS_HudGroupMap.GROUP_HEAD;
		}

		if (token == "голова")
		{
			return OMD_IDS_HudGroupMap.GROUP_HEAD;
		}

		if (token == "Торс")
		{
			return OMD_IDS_HudGroupMap.GROUP_TORSO;
		}

		if (token == "торс")
		{
			return OMD_IDS_HudGroupMap.GROUP_TORSO;
		}

		if (token == "Рука")
		{
			return OMD_IDS_HudGroupMap.GROUP_ARM;
		}

		if (token == "рука")
		{
			return OMD_IDS_HudGroupMap.GROUP_ARM;
		}

		if (token == "Кисть")
		{
			return OMD_IDS_HudGroupMap.GROUP_HAND;
		}

		if (token == "кисть")
		{
			return OMD_IDS_HudGroupMap.GROUP_HAND;
		}

		if (token == "Нога")
		{
			return OMD_IDS_HudGroupMap.GROUP_LEG;
		}

		if (token == "нога")
		{
			return OMD_IDS_HudGroupMap.GROUP_LEG;
		}

		if (token == "Стопа")
		{
			return OMD_IDS_HudGroupMap.GROUP_FOOT;
		}

		if (token == "стопа")
		{
			return OMD_IDS_HudGroupMap.GROUP_FOOT;
		}

		if (token == "Кость")
		{
			return OMD_IDS_HudGroupMap.GROUP_BONE;
		}

		if (token == "кость")
		{
			return OMD_IDS_HudGroupMap.GROUP_BONE;
		}

		if (token == "Лёгкие")
		{
			return OMD_IDS_HudGroupMap.GROUP_ORGAN;
		}

		if (token == "лёгкие")
		{
			return OMD_IDS_HudGroupMap.GROUP_ORGAN;
		}

		if (token == "Легкие")
		{
			return OMD_IDS_HudGroupMap.GROUP_ORGAN;
		}

		if (token == "легкие")
		{
			return OMD_IDS_HudGroupMap.GROUP_ORGAN;
		}

		if (token == "Орган")
		{
			return OMD_IDS_HudGroupMap.GROUP_ORGAN;
		}

		if (token == "орган")
		{
			return OMD_IDS_HudGroupMap.GROUP_ORGAN;
		}

		return "";
	}

	private static void PrintUsage()
	{
		Print("[OMD_IDS] ids_hud show <group>");
		Print("[OMD_IDS] ids_hud show <group> <label>");
		Print("[OMD_IDS] ids_hud hide <group>");
		Print("[OMD_IDS] ids_hud clear");
		Print("[OMD_IDS] ids_hud zones");
		Print("[OMD_IDS] ids_hud reload");
		Print("[OMD_IDS] same label stacks as xN, a different label adds another icon");
	}
};

#endif

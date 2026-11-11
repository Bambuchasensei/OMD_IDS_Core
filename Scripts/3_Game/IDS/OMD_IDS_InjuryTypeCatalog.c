// Type id, debug name and atlas badge. OrganWound has no badge: the group icon is the picture.
class OMD_IDS_InjuryTypeCatalog
{
	static const string LABEL_SCRATCH = "wound_scratch";
	static const string LABEL_LIGHT = "wound_light";
	static const string LABEL_DEEP = "wound_deep";
	static const string LABEL_BIG = "wound_big";
	static const string LABEL_BULLET = "wound_bullet";
	static const string LABEL_ARTERIAL = "arterial";
	static const string LABEL_BONE_BRUISE = "bone_bruise";
	static const string LABEL_BONE_STRESS = "bone_stress";
	static const string LABEL_BONE_CLOSED = "bone_closed";
	static const string LABEL_BONE_OPEN = "bone_open";
	static const string LABEL_SUTURED = "Зашито";
	static const string LABEL_DRESSING_CLEAN = "Забинтовано";
	static const string LABEL_DRESSING_DIRTY = "Повязка грязная";
	static const string LABEL_SPLINTED = "Шина";

	private static ref array<string> s_Names;

	static int FromName(string typeName)
	{
		Ensure();
		if (typeName == "")
		{
			return OMD_IDS_InjuryTypeId.NONE;
		}

		int index;
		for (index = 0; index < s_Names.Count(); index = index + 1)
		{
			if (s_Names.Get(index) == typeName)
			{
				return index;
			}
		}

		return OMD_IDS_InjuryTypeId.NONE;
	}

	static string GetName(int typeId)
	{
		Ensure();
		if (typeId < 0)
		{
			return "";
		}

		if (typeId >= s_Names.Count())
		{
			return "";
		}

		return s_Names.Get(typeId);
	}

	static bool IsKnown(int typeId)
	{
		if (typeId == OMD_IDS_InjuryTypeId.NONE)
		{
			return false;
		}

		string typeName = GetName(typeId);
		if (typeName == "")
		{
			return false;
		}

		return true;
	}

	static string GetLabel(int typeId)
	{
		if (typeId == OMD_IDS_InjuryTypeId.SCRATCH)
		{
			return LABEL_SCRATCH;
		}

		if (typeId == OMD_IDS_InjuryTypeId.LIGHT_WOUND)
		{
			return LABEL_LIGHT;
		}

		if (typeId == OMD_IDS_InjuryTypeId.DEEP_WOUND)
		{
			return LABEL_DEEP;
		}

		if (typeId == OMD_IDS_InjuryTypeId.BIG_WOUND)
		{
			return LABEL_BIG;
		}

		if (typeId == OMD_IDS_InjuryTypeId.BULLET)
		{
			return LABEL_BULLET;
		}

		if (typeId == OMD_IDS_InjuryTypeId.ARTERIAL)
		{
			return LABEL_ARTERIAL;
		}

		if (typeId == OMD_IDS_InjuryTypeId.BONE_BRUISE)
		{
			return LABEL_BONE_BRUISE;
		}

		if (typeId == OMD_IDS_InjuryTypeId.STRESS_FRACTURE)
		{
			return LABEL_BONE_STRESS;
		}

		if (typeId == OMD_IDS_InjuryTypeId.CLOSED_FRACTURE)
		{
			return LABEL_BONE_CLOSED;
		}

		if (typeId == OMD_IDS_InjuryTypeId.OPEN_FRACTURE)
		{
			return LABEL_BONE_OPEN;
		}

		if (typeId == OMD_IDS_InjuryTypeId.SUTURED)
		{
			return LABEL_SUTURED;
		}

		if (typeId == OMD_IDS_InjuryTypeId.DRESSING_CLEAN)
		{
			return LABEL_DRESSING_CLEAN;
		}

		if (typeId == OMD_IDS_InjuryTypeId.DRESSING_DIRTY)
		{
			return LABEL_DRESSING_DIRTY;
		}

		if (typeId == OMD_IDS_InjuryTypeId.SPLINTED)
		{
			return LABEL_SPLINTED;
		}

		return "";
	}

	static bool IsAtlasBadge(string label)
	{
		if (label == LABEL_SCRATCH)
		{
			return true;
		}

		if (label == LABEL_LIGHT)
		{
			return true;
		}

		if (label == LABEL_DEEP)
		{
			return true;
		}

		if (label == LABEL_BIG)
		{
			return true;
		}

		if (label == LABEL_BULLET)
		{
			return true;
		}

		if (label == LABEL_ARTERIAL)
		{
			return true;
		}

		if (label == LABEL_BONE_BRUISE)
		{
			return true;
		}

		if (label == LABEL_BONE_STRESS)
		{
			return true;
		}

		if (label == LABEL_BONE_CLOSED)
		{
			return true;
		}

		if (label == LABEL_BONE_OPEN)
		{
			return true;
		}

		if (label == LABEL_SUTURED)
		{
			return true;
		}

		if (label == LABEL_DRESSING_CLEAN)
		{
			return true;
		}

		if (label == LABEL_DRESSING_DIRTY)
		{
			return true;
		}

		if (label == LABEL_SPLINTED)
		{
			return true;
		}

		return false;
	}

	static bool IsWoundMark(string label)
	{
		if (label == LABEL_SCRATCH)
		{
			return true;
		}

		if (label == LABEL_LIGHT)
		{
			return true;
		}

		if (label == LABEL_DEEP)
		{
			return true;
		}

		if (label == LABEL_BIG)
		{
			return true;
		}

		if (label == LABEL_ARTERIAL)
		{
			return true;
		}

		return false;
	}

	static bool IsBoneBulletMark(string label)
	{
		if (label == LABEL_BULLET)
		{
			return true;
		}

		if (label == LABEL_BONE_BRUISE)
		{
			return true;
		}

		if (label == LABEL_BONE_STRESS)
		{
			return true;
		}

		if (label == LABEL_BONE_CLOSED)
		{
			return true;
		}

		if (label == LABEL_BONE_OPEN)
		{
			return true;
		}

		return false;
	}

	static string BadgePath(string label)
	{
		if (label == LABEL_ARTERIAL)
		{
			return "set:ids_icons image:wound_arterial";
		}

		if (label == LABEL_SUTURED)
		{
			return "";
		}

		if (label == LABEL_DRESSING_CLEAN)
		{
			return "";
		}

		if (label == LABEL_DRESSING_DIRTY)
		{
			return "";
		}

		if (label == LABEL_SPLINTED)
		{
			return "";
		}

		return "set:ids_icons image:" + label;
	}

	private static void Ensure()
	{
		if (s_Names)
		{
			return;
		}

		s_Names = new array<string>();
		Add(OMD_IDS_InjuryTypeId.NONE, "");
		Add(OMD_IDS_InjuryTypeId.SCRATCH, "Scratch");
		Add(OMD_IDS_InjuryTypeId.LIGHT_WOUND, "LightWound");
		Add(OMD_IDS_InjuryTypeId.DEEP_WOUND, "DeepWound");
		Add(OMD_IDS_InjuryTypeId.BIG_WOUND, "BigWound");
		Add(OMD_IDS_InjuryTypeId.BULLET, "Bullet");
		Add(OMD_IDS_InjuryTypeId.ORGAN_WOUND, "OrganWound");
		Add(OMD_IDS_InjuryTypeId.ARTERIAL, "Arterial");
		Add(OMD_IDS_InjuryTypeId.BONE_BRUISE, "BoneBruise");
		Add(OMD_IDS_InjuryTypeId.STRESS_FRACTURE, "StressFracture");
		Add(OMD_IDS_InjuryTypeId.CLOSED_FRACTURE, "ClosedFracture");
		Add(OMD_IDS_InjuryTypeId.OPEN_FRACTURE, "OpenFracture");
		Add(OMD_IDS_InjuryTypeId.SUTURED, "Sutured");
		Add(OMD_IDS_InjuryTypeId.DRESSING_CLEAN, "DressingClean");
		Add(OMD_IDS_InjuryTypeId.DRESSING_DIRTY, "DressingDirty");
		Add(OMD_IDS_InjuryTypeId.SPLINTED, "Splinted");
		if (s_Names.Count() != OMD_IDS_InjuryTypeId.SPLINTED + 1)
		{
			Print("[OMD_IDS] injury type catalog does not match OMD_IDS_InjuryTypeId");
		}
	}

	private static void Add(int typeId, string typeName)
	{
		if (s_Names.Count() != typeId)
		{
			Print(string.Format("[OMD_IDS] injury type catalog index drift at '%1'", typeName));
		}

		s_Names.Insert(typeName);
	}
};

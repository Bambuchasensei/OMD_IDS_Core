// String ids for HUD groups. OrganWound is a snapshot group, not a body zone.
class OMD_IDS_GroupCatalog
{
	private static ref array<string> s_Names;

	static string GetName(int groupId)
	{
		Ensure();
		if (groupId < 0)
		{
			return "";
		}

		if (groupId >= s_Names.Count())
		{
			return "";
		}

		return s_Names.Get(groupId);
	}

	private static void Ensure()
	{
		if (s_Names)
		{
			return;
		}

		s_Names = new array<string>();
		Add(OMD_IDS_GroupId.NONE, "");
		Add(OMD_IDS_GroupId.HEAD, "Head");
		Add(OMD_IDS_GroupId.TORSO, "Torso");
		Add(OMD_IDS_GroupId.ARM, "Arm");
		Add(OMD_IDS_GroupId.HAND, "Hand");
		Add(OMD_IDS_GroupId.LEG, "Leg");
		Add(OMD_IDS_GroupId.FOOT, "Foot");
		Add(OMD_IDS_GroupId.BONE, "Bone");
		Add(OMD_IDS_GroupId.LUNGS, "Organ");
		Add(OMD_IDS_GroupId.HEART, "Heart");
		Add(OMD_IDS_GroupId.LIVER, "Liver");
		Add(OMD_IDS_GroupId.KIDNEYS, "Kidneys");
		Add(OMD_IDS_GroupId.GUTS, "Guts");
		Add(OMD_IDS_GroupId.ORGAN_WOUND, "OrganWound");
		if (s_Names.Count() != OMD_IDS_GroupId.ORGAN_WOUND + 1)
		{
			Print("[OMD_IDS] group catalog does not match OMD_IDS_GroupId");
		}
	}

	private static void Add(int groupId, string groupName)
	{
		if (s_Names.Count() != groupId)
		{
			Print(string.Format("[OMD_IDS] group catalog index drift at '%1'", groupName));
		}

		s_Names.Insert(groupName);
	}
};

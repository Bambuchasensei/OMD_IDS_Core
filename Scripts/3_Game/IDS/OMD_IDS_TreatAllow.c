// One allow check for bandage, suture, splint, bullet extraction and dressing removal.
// A free function is not used: Enforce Script rejects static free functions.
// The client never loads injuries.json. Until Publish runs, every type allows only LeftHand and RightHand.
// The server calls Publish after it reads treatmentZones. The client calls Publish from the cfg gameplay packet.
// A missing key stays on those two hands. An empty list that was actually received allows nothing.
class OMD_IDS_TreatAllow
{
	static const int SPLINT = 1;
	static const int SUTURE = 2;
	static const int DRESSING = 3;
	static const int EXTRACT = 4;
	static const int REMOVE = 5;

	private static int s_Custom;
	private static ref array<string> s_Default;
	private static ref array<string> s_Splint;
	private static ref array<string> s_Suture;
	private static ref array<string> s_Dressing;
	private static ref array<string> s_Extract;
	private static ref array<string> s_Remove;

	// mode matches PickHand: 0 suture, 1 splint, 2 bandage, 3 remove dressing, 4 extract bullet.
	static int TypeOfMode(int mode)
	{
		if (mode == 1)
		{
			return SPLINT;
		}

		if (mode == 2)
		{
			return DRESSING;
		}

		if (mode == 3)
		{
			return REMOVE;
		}

		if (mode == 4)
		{
			return EXTRACT;
		}

		return SUTURE;
	}

	static int IsAllowed(int zoneId, int treatType)
	{
		string zoneName;

		zoneName = OMD_IDS_ZoneCatalog.GetName(zoneId);
		if (zoneName == "")
		{
			return 0;
		}

		return InList(zoneName, ListFor(treatType));
	}

	static void Publish(array<string> splint, array<string> suture, array<string> dressing, array<string> extractBullet, array<string> removeDressing)
	{
		s_Custom = 1;
		s_Splint = CopyList(splint);
		s_Suture = CopyList(suture);
		s_Dressing = CopyList(dressing);
		s_Extract = CopyList(extractBullet);
		s_Remove = CopyList(removeDressing);
	}

	private static array<string> ListFor(int treatType)
	{
		if (s_Custom == 0)
		{
			return DefaultHands();
		}

		if (treatType == SPLINT)
		{
			return s_Splint;
		}

		if (treatType == SUTURE)
		{
			return s_Suture;
		}

		if (treatType == DRESSING)
		{
			return s_Dressing;
		}

		if (treatType == EXTRACT)
		{
			return s_Extract;
		}

		if (treatType == REMOVE)
		{
			return s_Remove;
		}

		return DefaultHands();
	}

	private static array<string> DefaultHands()
	{
		if (!s_Default)
		{
			s_Default = new array<string>();
			s_Default.Insert("LeftHand");
			s_Default.Insert("RightHand");
		}

		return s_Default;
	}

	// null means the key was never set, so the old hand pair is kept.
	// A present empty array stays empty. The copy is not the shared default.
	private static array<string> CopyList(array<string> source)
	{
		int index;
		array<string> copy;

		copy = new array<string>();
		if (!source)
		{
			copy.Insert("LeftHand");
			copy.Insert("RightHand");
			return copy;
		}

		for (index = 0; index < source.Count(); index = index + 1)
		{
			copy.Insert(source.Get(index));
		}

		return copy;
	}

	private static int InList(string zoneName, array<string> names)
	{
		int index;
		string listed;

		if (!names)
		{
			return 0;
		}

		for (index = 0; index < names.Count(); index = index + 1)
		{
			listed = names.Get(index);
			if (listed == zoneName)
			{
				return 1;
			}
		}

		return 0;
	}
};

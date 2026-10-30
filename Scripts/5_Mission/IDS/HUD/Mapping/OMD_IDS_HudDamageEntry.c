#ifndef SERVER

class OMD_IDS_HudDamageEntry
{
	string groupId;
	string label;
	int count;

	OMD_IDS_HudDamageEntry Clone()
	{
		OMD_IDS_HudDamageEntry copy = new OMD_IDS_HudDamageEntry();
		copy.groupId = groupId;
		copy.label = label;
		copy.count = count;
		return copy;
	}
};

#endif

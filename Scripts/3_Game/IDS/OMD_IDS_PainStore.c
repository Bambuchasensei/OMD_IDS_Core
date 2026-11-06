// Client copy of pain levels. The server sends a fresh list only when a level changes.
// Zones missing from that list are level 0. HUD does not read this yet.
class OMD_IDS_PainStore
{
	private static ref OMD_IDS_PainStore s_Instance;

	private int m_BodyLevel;
	private ref array<int> m_ZoneLevel;

	static OMD_IDS_PainStore Get()
	{
		if (!s_Instance)
		{
			s_Instance = new OMD_IDS_PainStore();
		}

		return s_Instance;
	}

	void OMD_IDS_PainStore()
	{
		ResetArrays();
	}

	void Replace(int bodyLevel, array<int> zoneIds, array<int> levels)
	{
		int index;
		int count;
		int zoneId;
		int level;

		ResetArrays();
		m_BodyLevel = bodyLevel;
		if (m_BodyLevel < 0)
		{
			m_BodyLevel = 0;
		}

		if (m_BodyLevel > 4)
		{
			m_BodyLevel = 4;
		}

		if (!zoneIds)
		{
			return;
		}

		if (!levels)
		{
			return;
		}

		count = zoneIds.Count();
		if (levels.Count() < count)
		{
			count = levels.Count();
		}

		for (index = 0; index < count; index = index + 1)
		{
			zoneId = zoneIds.Get(index);
			level = levels.Get(index);
			if (zoneId < 1)
			{
				continue;
			}

			if (zoneId > OMD_IDS_ZoneId.GUTS)
			{
				continue;
			}

			if (level < 0)
			{
				level = 0;
			}

			if (level > 4)
			{
				level = 4;
			}

			m_ZoneLevel.Set(zoneId, level);
		}
	}

	int GetBodyLevel()
	{
		return m_BodyLevel;
	}

	int GetZoneLevel(int zoneId)
	{
		if (!m_ZoneLevel)
		{
			return 0;
		}

		if (zoneId < 0)
		{
			return 0;
		}

		if (zoneId >= m_ZoneLevel.Count())
		{
			return 0;
		}

		return m_ZoneLevel.Get(zoneId);
	}

	protected void ResetArrays()
	{
		int zoneId;

		m_BodyLevel = 0;
		m_ZoneLevel = new array<int>();
		for (zoneId = 0; zoneId <= OMD_IDS_ZoneId.GUTS; zoneId = zoneId + 1)
		{
			m_ZoneLevel.Insert(0);
		}
	}
};

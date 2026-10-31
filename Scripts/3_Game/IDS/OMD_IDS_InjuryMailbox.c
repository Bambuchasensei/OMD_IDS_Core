// 4_World leaves the snapshot here. 5_Mission reads it on the next frame.
// A free global is not used: this is the same singleton shape as the HUD manager.
class OMD_IDS_InjuryMailbox
{
	private static ref OMD_IDS_InjuryMailbox s_Instance;

	private ref array<ref OMD_IDS_InjurySnapshotEntry> m_Pending;
	private bool m_HasPending;

	static OMD_IDS_InjuryMailbox Get()
	{
		if (!s_Instance)
		{
			s_Instance = new OMD_IDS_InjuryMailbox();
		}

		return s_Instance;
	}

	void Put(array<ref OMD_IDS_InjurySnapshotEntry> entries)
	{
		m_Pending = entries;
		m_HasPending = true;
	}

	bool HasPending()
	{
		return m_HasPending;
	}

	array<ref OMD_IDS_InjurySnapshotEntry> Take()
	{
		m_HasPending = false;
		array<ref OMD_IDS_InjurySnapshotEntry> entries = m_Pending;
		m_Pending = null;
		if (!entries)
		{
			entries = new array<ref OMD_IDS_InjurySnapshotEntry>();
		}

		return entries;
	}
};

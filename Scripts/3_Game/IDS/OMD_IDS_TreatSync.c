// Rides inside vanilla CfgGameplayJson. The server fills the fields in OMD_IDS_InjuryConfig.Store.
// The client applies them from PlayerBase.OnGameplayDataHandlerSync.
// Arrays stay arrays. zonesProbe is the sum of the five lists, so a dropped array is visible in the log.
// Parent is ITEM_DataBase, the same class vanilla cfg gameplay data uses.
// P:\scripts\3_game\cfggameplaydatajson.c:26
// Json_DataBase is not in the scripts.
class OMD_IDS_TreatSync : ITEM_DataBase
{
	int m_Ready;
	float extractSec;
	float sutureSec;
	float removeDressingSec;
	ref array<string> splint = new array<string>();
	ref array<string> suture = new array<string>();
	ref array<string> dressing = new array<string>();
	ref array<string> extractBullet = new array<string>();
	ref array<string> removeDressing = new array<string>();
	int zonesProbe;

	// Called from the client player after super.OnGameplayDataHandlerSync.
	// m_Ready 0 leaves the factories. A lost array keeps those zone factories and still takes the three times.
	void ApplyOnClient()
	{
		int splintN;
		int sutureN;
		int dressingN;
		int extractN;
		int removeN;
		int nullCount;
		int sum;
		int lost;
		string line;

		splintN = 0;
		sutureN = 0;
		dressingN = 0;
		extractN = 0;
		removeN = 0;
		nullCount = 0;
		if (!splint)
		{
			nullCount = nullCount + 1;
		}
		else
		{
			splintN = splint.Count();
		}

		if (!suture)
		{
			nullCount = nullCount + 1;
		}
		else
		{
			sutureN = suture.Count();
		}

		if (!dressing)
		{
			nullCount = nullCount + 1;
		}
		else
		{
			dressingN = dressing.Count();
		}

		if (!extractBullet)
		{
			nullCount = nullCount + 1;
		}
		else
		{
			extractN = extractBullet.Count();
		}

		if (!removeDressing)
		{
			nullCount = nullCount + 1;
		}
		else
		{
			removeN = removeDressing.Count();
		}

		sum = splintN + sutureN + dressingN + extractN + removeN;
		line = string.Format("[OMD_IDS][CfgSync] client ready=%1 extract=%2 suture=%3 remove=%4 zones splint=%5 suture=%6 dressing=%7 extract=%8 remove=%9 ", m_Ready, extractSec, sutureSec, removeDressingSec, splintN, sutureN, dressingN, extractN, removeN);
		line = line + string.Format("probe=%1 arraysNull=%2.", zonesProbe, nullCount);
		Print(line);
		if (m_Ready != 1)
		{
			return;
		}

		OMD_IDS_TreatTimes.Apply(extractSec, sutureSec, removeDressingSec);
		lost = 0;
		if (zonesProbe > 0)
		{
			if (sum == 0)
			{
				lost = 1;
			}

			if (nullCount > 0)
			{
				lost = 1;
			}
		}

		if (lost == 1)
		{
			Print("[OMD_IDS][CfgSync] ARRAYS LOST, client stays on default zones.");
			return;
		}

		if (nullCount > 0)
		{
			return;
		}

		OMD_IDS_TreatAllow.Publish(splint, suture, dressing, extractBullet, removeDressing);
	}
};

modded class CfgGameplayJson
{
	ref OMD_IDS_TreatSync m_OMD_IDS_Treat = new OMD_IDS_TreatSync();
};

modded class CfgGameplayHandler
{
	static OMD_IDS_TreatSync GetOMD_IDS_Treat()
	{
		if (!m_Data)
		{
			return null;
		}

		if (!m_Data.m_OMD_IDS_Treat)
		{
			return null;
		}

		return m_Data.m_OMD_IDS_Treat;
	}

	// JsonFileLoader can null the field when cfggameplay.json is read. Store calls this and fills the new object.
	static OMD_IDS_TreatSync OMD_IDS_EnsureTreat()
	{
		OMD_IDS_TreatSync sync;

		if (!m_Data)
		{
			return null;
		}

		sync = m_Data.m_OMD_IDS_Treat;
		if (!sync)
		{
			sync = new OMD_IDS_TreatSync();
			m_Data.m_OMD_IDS_Treat = sync;
		}

		return sync;
	}
};

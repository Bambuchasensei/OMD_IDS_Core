// Factory times until the cfg gameplay packet is applied. The server action still reads injuries.json.
// Until Apply runs: extract 10, suture 10, remove dressing 3.
class OMD_IDS_TreatTimes
{
	private static int s_Set;
	private static float s_Extract;
	private static float s_Suture;
	private static float s_Remove;

	static float ExtractSec()
	{
		if (s_Set == 0)
		{
			return 10;
		}

		if (s_Extract < 0)
		{
			return 0;
		}

		return s_Extract;
	}

	static float SutureSec()
	{
		if (s_Set == 0)
		{
			return 10;
		}

		if (s_Suture < 0)
		{
			return 0;
		}

		return s_Suture;
	}

	static float RemoveDressingSec()
	{
		if (s_Set == 0)
		{
			return 3;
		}

		if (s_Remove < 0)
		{
			return 0;
		}

		return s_Remove;
	}

	static void Apply(float extractSec, float sutureSec, float removeSec)
	{
		s_Set = 1;
		s_Extract = extractSec;
		s_Suture = sutureSec;
		s_Remove = removeSec;
	}
};

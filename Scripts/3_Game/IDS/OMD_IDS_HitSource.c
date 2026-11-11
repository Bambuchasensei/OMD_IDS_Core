// Why a hit reached the HitFX socket. Both sides can read the number.
// The client hook does not need it today. OMD_IDS_HitFX may.
// Append only. 0 is a real source, not "no hit".
enum OMD_IDS_HitSource
{
	ZOMBIE = 0,
	MELEE = 1,
	BULLET = 2,
	FIST_SELF = 3,
	FALL = 4,
	OTHER = 5
}

class OMD_IDS_HitSourceName
{
	static string Name(int source)
	{
		if (source == OMD_IDS_HitSource.ZOMBIE)
		{
			return "ZOMBIE";
		}

		if (source == OMD_IDS_HitSource.MELEE)
		{
			return "MELEE";
		}

		if (source == OMD_IDS_HitSource.BULLET)
		{
			return "BULLET";
		}

		if (source == OMD_IDS_HitSource.FIST_SELF)
		{
			return "FIST_SELF";
		}

		if (source == OMD_IDS_HitSource.FALL)
		{
			return "FALL";
		}

		return "OTHER";
	}
};

// One formula for both hands. Callers pass flags they already collected.
// Bit 1 deep or big wound, 2 closed or open fracture, 4 bullet, 8 dressing, 16 suture, 32 splint.
// Scratch and a light wound are not bits. The contribution is the largest candidate, not a sum.
// Factory numbers live here. FillMissing on the server reads these. The client has no json.
class OMD_IDS_HandActionLoad
{
	static float FactoryDeep()
	{
		return 30;
	}

	static float FactoryFracture()
	{
		return 50;
	}

	static float FactoryBullet()
	{
		return 50;
	}

	static float FactoryMult()
	{
		return 1.4;
	}

	static int Pack(int deep, int fracture, int bullet, int dressing, int suture, int splint)
	{
		int flags;

		flags = 0;
		if (deep != 0)
		{
			flags = flags + 1;
		}

		if (fracture != 0)
		{
			flags = flags + 2;
		}

		if (bullet != 0)
		{
			flags = flags + 4;
		}

		if (dressing != 0)
		{
			flags = flags + 8;
		}

		if (suture != 0)
		{
			flags = flags + 16;
		}

		if (splint != 0)
		{
			flags = flags + 32;
		}

		return flags;
	}

	static void Loads(int leftFlags, int rightFlags, float deepVal, float fractureVal, float bulletVal, out float leftLoad, out float rightLoad)
	{
		leftLoad = One(leftFlags, deepVal, fractureVal, bulletVal);
		rightLoad = One(rightFlags, deepVal, fractureVal, bulletVal);
	}

	// 1 when neither hand or both hands contribute. The configured multiplier when exactly one does.
	// A stored 0 would collapse the timer, so it is treated as "do not stretch".
	static float TimeMult(float leftLoad, float rightLoad, float configured)
	{
		int one;

		one = 0;
		if (leftLoad > 0)
		{
			if (rightLoad <= 0)
			{
				one = 1;
			}
		}

		if (rightLoad > 0)
		{
			if (leftLoad <= 0)
			{
				one = 1;
			}
		}

		if (one == 0)
		{
			return 1;
		}

		if (configured <= 0)
		{
			return 1;
		}

		return configured;
	}

	static string ModeName(float leftLoad, float rightLoad)
	{
		if (leftLoad > 0)
		{
			if (rightLoad > 0)
			{
				return "both";
			}

			return "one";
		}

		if (rightLoad > 0)
		{
			return "one";
		}

		return "none";
	}

	private static float One(int flags, float deepVal, float fractureVal, float bulletVal)
	{
		float load;

		load = 0;
		if ((flags & 1) != 0)
		{
			if ((flags & 8) == 0)
			{
				if ((flags & 16) == 0)
				{
					if (deepVal > load)
					{
						load = deepVal;
					}
				}
			}
		}

		if ((flags & 2) != 0)
		{
			if ((flags & 32) == 0)
			{
				if (fractureVal > load)
				{
					load = fractureVal;
				}
			}
		}

		if ((flags & 4) != 0)
		{
			if (bulletVal > load)
			{
				load = bulletVal;
			}
		}

		if (load < 0)
		{
			load = 0;
		}

		return load;
	}
};

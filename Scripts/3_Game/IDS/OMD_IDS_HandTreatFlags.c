// Public hand bits for other players. Not saved. The server packs them.
// Bits 0-3 wound type, 4-7 bone type, 8-11 bullet count, 12-13 dressing, 14 suture, 15 splint.
// Dressing: 0 none, 1 clean, 2 dirty. Mode matches PickHand: 0 suture, 1 splint, 2 bandage, 3 remove, 4 extract.
// Ready() is one zone. The menu order is OMD_IDS_LimbCatalog.Pick. Lower HP is not used.
class OMD_IDS_HandTreatFlags
{
	static int Pack(int woundType, int boneType, int bulletCount, int dressingKind, int suture, int splint)
	{
		int flags;
		int wound;
		int bone;
		int bullets;
		int dressing;

		wound = woundType;
		if (wound < 0)
		{
			wound = 0;
		}

		if (wound > 15)
		{
			wound = 15;
		}

		bone = boneType;
		if (bone < 0)
		{
			bone = 0;
		}

		if (bone > 15)
		{
			bone = 15;
		}

		bullets = bulletCount;
		if (bullets < 0)
		{
			bullets = 0;
		}

		if (bullets > 15)
		{
			bullets = 15;
		}

		dressing = dressingKind;
		if (dressing < 0)
		{
			dressing = 0;
		}

		if (dressing > 3)
		{
			dressing = 3;
		}

		flags = wound;
		flags = flags + (bone << 4);
		flags = flags + (bullets << 8);
		flags = flags + (dressing << 12);
		if (suture != 0)
		{
			flags = flags + (1 << 14);
		}

		if (splint != 0)
		{
			flags = flags + (1 << 15);
		}

		return flags;
	}

	static int Wound(int flags)
	{
		int value;

		value = flags & 15;
		return value;
	}

	static int Bone(int flags)
	{
		int value;

		value = flags >> 4;
		value = value & 15;
		return value;
	}

	static int Bullets(int flags)
	{
		int value;

		value = flags >> 8;
		value = value & 15;
		return value;
	}

	static int Dressing(int flags)
	{
		int value;

		value = flags >> 12;
		value = value & 3;
		return value;
	}

	static int HasSuture(int flags)
	{
		int value;

		value = flags >> 14;
		value = value & 1;
		return value;
	}

	static int HasSplint(int flags)
	{
		int value;

		value = flags >> 15;
		value = value & 1;
		return value;
	}

	static int BandageOk(int flags)
	{
		int wound;

		wound = Wound(flags);
		if (wound != OMD_IDS_InjuryTypeId.SCRATCH)
		{
			if (wound != OMD_IDS_InjuryTypeId.LIGHT_WOUND)
			{
				if (wound != OMD_IDS_InjuryTypeId.DEEP_WOUND)
				{
					if (wound != OMD_IDS_InjuryTypeId.BIG_WOUND)
					{
						return 0;
					}
				}
			}
		}

		if (Dressing(flags) == 1)
		{
			return 0;
		}

		return 1;
	}

	static int SutureOk(int flags)
	{
		int wound;

		wound = Wound(flags);
		if (wound != OMD_IDS_InjuryTypeId.DEEP_WOUND)
		{
			if (wound != OMD_IDS_InjuryTypeId.BIG_WOUND)
			{
				return 0;
			}
		}

		if (HasSuture(flags) != 0)
		{
			return 0;
		}

		if (Dressing(flags) != 0)
		{
			return 0;
		}

		return 1;
	}

	static int ExtractOk(int flags)
	{
		if (Bullets(flags) < 1)
		{
			return 0;
		}

		if (Dressing(flags) != 0)
		{
			return 0;
		}

		return 1;
	}

	static int SplintOk(int flags)
	{
		int bone;

		bone = Bone(flags);
		if (bone != OMD_IDS_InjuryTypeId.CLOSED_FRACTURE)
		{
			if (bone != OMD_IDS_InjuryTypeId.OPEN_FRACTURE)
			{
				return 0;
			}
		}

		if (HasSplint(flags) != 0)
		{
			return 0;
		}

		return 1;
	}

	static int RemoveOk(int flags)
	{
		int dressing;

		dressing = Dressing(flags);
		if (dressing == 1)
		{
			return 1;
		}

		if (dressing == 2)
		{
			return 1;
		}

		return 0;
	}

	static int Ready(int flags, int mode)
	{
		if (mode == 3)
		{
			return RemoveOk(flags);
		}

		if (mode == 1)
		{
			return SplintOk(flags);
		}

		if (mode == 2)
		{
			return BandageOk(flags);
		}

		if (mode == 4)
		{
			return ExtractOk(flags);
		}

		return SutureOk(flags);
	}

	static int Count(int leftFlags, int rightFlags, int mode)
	{
		int total;

		total = 0;
		if (Ready(leftFlags, mode) != 0)
		{
			total = total + 1;
		}

		if (Ready(rightFlags, mode) != 0)
		{
			total = total + 1;
		}

		return total;
	}

	// 0 none, 1 left, 2 right. Both ready: variant 0 is left, variant 1 is right.
	static int SideReady(int leftOk, int rightOk, int variantId)
	{
		if (leftOk == 0)
		{
			if (rightOk == 0)
			{
				return 0;
			}

			if (variantId != 0)
			{
				return 0;
			}

			return 2;
		}

		if (rightOk == 0)
		{
			if (variantId != 0)
			{
				return 0;
			}

			return 1;
		}

		if (variantId == 0)
		{
			return 1;
		}

		if (variantId == 1)
		{
			return 2;
		}

		return 0;
	}

	static int Side(int leftFlags, int rightFlags, int variantId, int mode)
	{
		int leftOk;
		int rightOk;

		leftOk = Ready(leftFlags, mode);
		rightOk = Ready(rightFlags, mode);
		return SideReady(leftOk, rightOk, variantId);
	}

	static int Zone(int side)
	{
		if (side == 1)
		{
			return OMD_IDS_ZoneId.LEFT_HAND;
		}

		if (side == 2)
		{
			return OMD_IDS_ZoneId.RIGHT_HAND;
		}

		return 0;
	}

	static int Group(int side)
	{
		if (side == 1)
		{
			return OMD_IDS_GroupId.LEFT_HAND;
		}

		if (side == 2)
		{
			return OMD_IDS_GroupId.RIGHT_HAND;
		}

		return 0;
	}
};

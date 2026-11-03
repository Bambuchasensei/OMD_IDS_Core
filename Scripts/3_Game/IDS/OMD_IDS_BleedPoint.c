// External zone id to a vanilla bleeding bone.
// Orientation and offset are copied from the vanilla source registration.
// Torso meat uses Spine3, Spine2, Spine1 and Spine. Groin uses Pelvis.
// Zones without a matching bone are absent on purpose.
class OMD_IDS_BleedPoint
{
	static bool Find(int zoneId, out string bone, out vector orientation, out vector offset)
	{
		bone = "";
		orientation = "0 0 0";
		offset = "0 0 0";
		if (zoneId == OMD_IDS_ZoneId.HEAD)
		{
			bone = "Head";
			orientation = "95 90 0";
			offset = "0.15 -0.08 0.00";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.NECK)
		{
			bone = "Neck";
			orientation = "-130 70 0";
			offset = "0.02 -0.05 0.05";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.PELVIS)
		{
			bone = "Pelvis";
			orientation = "0 -135 0";
			offset = "0 0.12 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_SHOULDER)
		{
			bone = "LeftShoulder";
			orientation = "0 -90 0";
			offset = "0 0.07 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_SHOULDER)
		{
			bone = "RightShoulder";
			orientation = "0 90 0";
			offset = "0 -0.07 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_UPPER_ARM)
		{
			bone = "LeftArm";
			orientation = "0 90 90";
			offset = "0 -0.05 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_UPPER_ARM)
		{
			bone = "RightArm";
			orientation = "0 -90 0";
			offset = "0 0.05 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_LOWER_ARM)
		{
			bone = "LeftForeArm";
			orientation = "0 90 0";
			offset = "0 -0.05 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_LOWER_ARM)
		{
			bone = "RightForeArm";
			orientation = "0 -90 0";
			offset = "0 0.05 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_HAND)
		{
			bone = "LeftForeArmRoll";
			orientation = "0 90 0";
			offset = "0.1 0 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_HAND)
		{
			bone = "RightForeArmRoll";
			orientation = "0 90 0";
			offset = "-0.1 0 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_UPPER_LEG)
		{
			bone = "LeftUpLeg";
			orientation = "0 90 0";
			offset = "0 -0.12 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_UPPER_LEG)
		{
			bone = "RightUpLeg";
			orientation = "0 -90 0";
			offset = "0 0.12 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_LOWER_LEG)
		{
			bone = "LeftLeg";
			orientation = "0 90 0";
			offset = "0 -0.07 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_LOWER_LEG)
		{
			bone = "RightLeg";
			orientation = "0 -90 0";
			offset = "0 0.06 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.LEFT_FOOT)
		{
			bone = "LeftFoot";
			orientation = "0 180 0";
			offset = "0 0 0.035";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.RIGHT_FOOT)
		{
			bone = "RightFoot";
			orientation = "0 0 0";
			offset = "0 0 -0.035";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.UPPER_CHEST)
		{
			bone = "Spine3";
			orientation = "0 90 0";
			offset = "0 -0.05 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.MIDDLE_CHEST)
		{
			bone = "Spine2";
			orientation = "0 55 0";
			offset = "0 -0.07 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.LOWER_CHEST)
		{
			bone = "Spine1";
			orientation = "0 155 0";
			offset = "0 -0.1 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.UPPER_ABDOMEN)
		{
			bone = "Spine";
			orientation = "0 55 0";
			offset = "0 -0.1 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.LOWER_ABDOMEN)
		{
			bone = "Spine";
			orientation = "0 55 0";
			offset = "0 -0.1 0";
			return true;
		}

		if (zoneId == OMD_IDS_ZoneId.GROIN)
		{
			bone = "Pelvis";
			orientation = "0 -135 0";
			offset = "0 0.12 0";
			return true;
		}

		return false;
	}
};

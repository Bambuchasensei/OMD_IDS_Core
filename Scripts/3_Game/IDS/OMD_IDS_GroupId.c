// HUD group ids shared by the server snapshot and the client bridge.
// Append only. Names live in OMD_IDS_GroupCatalog and must match the HUD json ids.
// 0 NONE, 1 HEAD, 2 TORSO, 3 ARM, 4 HAND, 5 LEG, 6 FOOT, 7 BONE,
// 8 LUNGS is the shared organ group «Орган» (lungs icon). 9-12 stay unused. 13 ORGAN_WOUND
// 14 LEFT_HAND and 15 RIGHT_HAND. HAND stays 4 so an old snapshot row still has a name.
// 16 LEFT_FOOT and 17 RIGHT_FOOT. FOOT stays 6 so an old snapshot row still has a name.
// 18 UPPER_CHEST, 19 MIDDLE_CHEST, 20 PELVIS, 21 LEFT_SHOULDER, 22 RIGHT_SHOULDER.
// 23 LOWER_CHEST, 24 UPPER_ABDOMEN, 25 LOWER_ABDOMEN, 26 GROIN.
// Groups 18-26 draw on the Torso picture. TORSO stays 2.
// 27 LEFT_UPPER_ARM, 28 LEFT_LOWER_ARM, 29 RIGHT_UPPER_ARM, 30 RIGHT_LOWER_ARM.
// 31 LEFT_UPPER_LEG, 32 LEFT_LOWER_LEG, 33 RIGHT_UPPER_LEG, 34 RIGHT_LOWER_LEG.
// Groups 27-30 draw on the Arm picture. Groups 31-34 draw on the Leg picture. ARM stays 3. LEG stays 5.
enum OMD_IDS_GroupId
{
	NONE = 0,
	HEAD = 1,
	TORSO = 2,
	ARM = 3,
	HAND = 4,
	LEG = 5,
	FOOT = 6,
	BONE = 7,
	LUNGS = 8,
	HEART = 9,
	LIVER = 10,
	KIDNEYS = 11,
	GUTS = 12,
	ORGAN_WOUND = 13,
	LEFT_HAND = 14,
	RIGHT_HAND = 15,
	LEFT_FOOT = 16,
	RIGHT_FOOT = 17,
	UPPER_CHEST = 18,
	MIDDLE_CHEST = 19,
	PELVIS = 20,
	LEFT_SHOULDER = 21,
	RIGHT_SHOULDER = 22,
	LOWER_CHEST = 23,
	UPPER_ABDOMEN = 24,
	LOWER_ABDOMEN = 25,
	GROIN = 26,
	LEFT_UPPER_ARM = 27,
	LEFT_LOWER_ARM = 28,
	RIGHT_UPPER_ARM = 29,
	RIGHT_LOWER_ARM = 30,
	LEFT_UPPER_LEG = 31,
	LEFT_LOWER_LEG = 32,
	RIGHT_UPPER_LEG = 33,
	RIGHT_LOWER_LEG = 34
}

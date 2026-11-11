// HUD group ids shared by the server snapshot and the client bridge.
// Append only. Names live in OMD_IDS_GroupCatalog and must match the HUD json ids.
// 0 NONE, 1 HEAD, 2 TORSO, 3 ARM, 4 HAND, 5 LEG, 6 FOOT, 7 BONE,
// 8 LUNGS is the shared organ group «Орган» (lungs icon). 9-12 stay unused. 13 ORGAN_WOUND
// 14 LEFT_HAND and 15 RIGHT_HAND. HAND stays 4 so an old snapshot row still has a name.
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
	RIGHT_HAND = 15
}

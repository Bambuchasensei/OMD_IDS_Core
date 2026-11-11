// Bidirectional stub. The server mod replaces these methods.
// One instance. Actions call Get(). There is no free function.
class OMD_IDS_TreatmentHooks
{
	static const int OP_EXTRACT = 1;
	static const int OP_SUTURE = 2;
	static const int OP_BANDAGE = 3;
	static const int OP_REMOVE = 4;
	static const int OP_SPLINT = 5;
	static const int RESULT_OK = 1;
	static const int RESULT_FAIL = 2;
	static const int RESULT_CHANGE = 3;
	private static ref OMD_IDS_TreatmentHooks s_Instance;

	static OMD_IDS_TreatmentHooks Get()
	{
		if (!s_Instance)
		{
			s_Instance = new OMD_IDS_TreatmentHooks();
		}

		return s_Instance;
	}

	void OnSuture(PlayerBase actor, PlayerBase patient, int zoneId, bool success)
	{
	}

	void OnSplint(PlayerBase actor, PlayerBase patient, int zoneId)
	{
	}

	void OnAnalgesic(PlayerBase patient, string drugId)
	{
	}

	bool CanSutureServer(PlayerBase patient, int zoneId)
	{
		return false;
	}

	bool CanSplintServer(PlayerBase patient, int zoneId)
	{
		return false;
	}

	// 0 means "no hand". The server override picks the real zone.
	int SutureZone(PlayerBase patient, int variantId)
	{
		return 0;
	}

	int SplintZone(PlayerBase patient, int variantId)
	{
		return 0;
	}

	// Defaults match heal.sutureChanceSelf / sutureChanceOther / the two bonuses.
	// The server override reads injuries.json. A stored 0 stays 0.
	float SutureChanceSelf()
	{
		return 30;
	}

	float SutureChanceOther()
	{
		return 40;
	}

	// Client stub. The server override reads activeEnabled.
	bool BandageActive()
	{
		return false;
	}

	void OnBandage(PlayerBase actor, PlayerBase patient, int zoneId, ItemBase item)
	{
	}

	bool CanBandageServer(PlayerBase patient, int zoneId)
	{
		return false;
	}

	int BandageZone(PlayerBase patient, int variantId)
	{
		return 0;
	}

	bool CanRemoveDressingServer(PlayerBase patient, int zoneId)
	{
		return false;
	}

	int RemoveDressingZone(PlayerBase patient, int variantId)
	{
		return 0;
	}

	void OnRemoveDressing(PlayerBase actor, PlayerBase patient, int zoneId)
	{
	}

	// Client defaults. The server override reads heal.sutureShockTotal and heal.sutureShockTickSec.
	float SutureShockTotal()
	{
		return 50;
	}

	float SutureShockTickSec()
	{
		return 1;
	}

	void BeginSutureShock(PlayerBase patient, int zoneId)
	{
	}

	void ApplySutureShock(PlayerBase patient, int zoneId, float portion)
	{
	}

	void EndSutureShock(PlayerBase patient, int zoneId)
	{
	}

	bool CanExtractServer(PlayerBase patient, int zoneId)
	{
		return false;
	}

	int ExtractZone(PlayerBase patient, int variantId)
	{
		return 0;
	}

	// Client value arrives in the cfg gameplay packet. Until then the factory is 10. The server override reads injuries.json. A stored 0 stays 0.
	float ExtractDurationSec()
	{
		return OMD_IDS_TreatTimes.ExtractSec();
	}

	// Client value arrives in the cfg gameplay packet. Until then the factory is 10. The server override reads injuries.json. A stored 0 stays 0.
	float SutureDurationSec()
	{
		return OMD_IDS_TreatTimes.SutureSec();
	}

	// Client value arrives in the cfg gameplay packet. Until then the factory is 3. The server override reads injuries.json. A stored 0 stays 0.
	float RemoveDressingDurationSec()
	{
		return OMD_IDS_TreatTimes.RemoveDressingSec();
	}

	float ExtractShockTotal()
	{
		return 70;
	}

	float ExtractShockTickSec()
	{
		return 2.0;
	}

	float ExtractChance(ItemBase knife)
	{
		return 0.3;
	}

	float ExtractKnifeDamage(ItemBase knife)
	{
		return 5;
	}

	void BeginExtractShock(PlayerBase patient, int zoneId)
	{
	}

	void ApplyExtractShock(PlayerBase patient, int zoneId, float portion)
	{
	}

	void EndExtractShock(PlayerBase patient, int zoneId)
	{
	}

	void OnExtract(PlayerBase actor, PlayerBase patient, int zoneId, ItemBase knife)
	{
	}

	// Server tells the healer why the finished action was refused. The client stub does nothing.
	void OnTreatRefused(PlayerBase actor, PlayerBase patient, int zoneId, int op)
	{
	}

	// Client reads the owner snapshot and the factory numbers. The server mod replaces this.
	void HandActionLoads(PlayerBase player, out float leftLoad, out float rightLoad, out float configuredMult)
	{
		int leftDeep;
		int leftFracture;
		int leftBullet;
		int leftDressing;
		int leftSuture;
		int leftSplint;
		int rightDeep;
		int rightFracture;
		int rightBullet;
		int rightDressing;
		int rightSuture;
		int rightSplint;
		int leftFlags;
		int rightFlags;

		leftLoad = 0;
		rightLoad = 0;
		configuredMult = OMD_IDS_HandActionLoad.FactoryMult();
		if (!player)
		{
			return;
		}

		OMD_IDS_HandSnapshot.ActionFlags(OMD_IDS_GroupId.LEFT_HAND, leftDeep, leftFracture, leftBullet, leftDressing, leftSuture, leftSplint);
		OMD_IDS_HandSnapshot.ActionFlags(OMD_IDS_GroupId.RIGHT_HAND, rightDeep, rightFracture, rightBullet, rightDressing, rightSuture, rightSplint);
		leftFlags = OMD_IDS_HandActionLoad.Pack(leftDeep, leftFracture, leftBullet, leftDressing, leftSuture, leftSplint);
		rightFlags = OMD_IDS_HandActionLoad.Pack(rightDeep, rightFracture, rightBullet, rightDressing, rightSuture, rightSplint);
		OMD_IDS_HandActionLoad.Loads(leftFlags, rightFlags, OMD_IDS_HandActionLoad.FactoryDeep(), OMD_IDS_HandActionLoad.FactoryFracture(), OMD_IDS_HandActionLoad.FactoryBullet(), leftLoad, rightLoad);
	}

	void OnHandActionStart(ActionData action_data)
	{
	}
};

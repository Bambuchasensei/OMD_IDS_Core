class CfgPatches
{
	class OMD_IDS_Core
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Characters",
			"DZ_Scripts"
		};
	};
};

class CfgMods
{
	class OMD_IDS_Core
	{
		dir = "OMD_IDS_Core";
		picture = "";
		action = "";
		hideName = 0;
		hidePicture = 0;
		name = "OMD IDS Core";
		credits = "Organ hitbox model: Genriet1 STEAM_URL";
		author = "OMD";
		authorID = "0";
		version = "0.1.0";
		type = "mod";
		dependencies[] = {"Game", "World", "Mission"};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"OMD_IDS_Core/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"OMD_IDS_Core/Scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"OMD_IDS_Core/Scripts/5_Mission"};
			};
			class imageSets
			{
				files[] = {"OMD_IDS_Core/gui/imagesets/ids_icons.imageset"};
			};
		};
	};
};

class CfgVehicles
{
	class Man;
	class SurvivorBase: Man
	{
		class DamageSystem
		{
			class DamageZones
			{
				// ===== Head / Neck =====
				class Head
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_head"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Neck
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_neck"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};

				// ===== Torso =====
				class UpperChest
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_upperChest"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class MiddleChest
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_middleChest"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LowerChest
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_lowerChest"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class UpperAbdomen
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_upperAbdomen"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LowerAbdomen
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_lowerAbdomen"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Pelvis
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_pelvis"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Groin
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_groin"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};

				// ===== Left arm =====
				class LeftShoulder
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftShoulder"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LeftUpperArm
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftUpperArm"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LeftUpperArmBone
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftUpperArmBone"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LeftLowerArm
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftLowerArm"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LeftLowerArmBone
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftLowerArmBone"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LeftHand
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftHand"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};

				// ===== Right arm =====
				class RightShoulder
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightShoulder"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class RightUpperArm
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightUpperArm"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class RightUpperArmBone
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightUpperArmBone"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class RightLowerArm
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightLowerArm"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class RightLowerArmBone
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightLowerArmBone"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class RightHand
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightHand"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};

				// ===== Left leg =====
				class LeftUpperLeg
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftUpperLeg"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LeftUpperLegBone
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftUpperLegBone"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LeftLowerLeg
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftLowerLeg"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LeftLowerLegBone
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftLowerLegBone"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class LeftFoot
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_leftFoot"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};

				// ===== Right leg =====
				class RightUpperLeg
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightUpperLeg"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class RightUpperLegBone
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightUpperLegBone"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class RightLowerLeg
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightLowerLeg"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class RightLowerLegBone
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightLowerLegBone"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class RightFoot
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_rightFoot"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};

				// ===== Organs =====
				class Brain
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 5; }; class Shock { damage = 5; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_brain"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Spine
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 5; }; class Shock { damage = 5; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_spine"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Heart
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_heart"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Lungs
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_lungs"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Liver
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_liver"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Kidneys
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_kidneys"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
				class Guts
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 1; }; class Shock { damage = 1; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
					componentNames[] = {"dmgZone_guts"};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
                class Torso 
                {	
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 0; }; class Shock { damage = 0; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
                    componentNames[] = {};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
                };
                class LeftArm 
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 0; }; class Shock { damage = 0; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
                    componentNames[] = {};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
                class RightArm 
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 0; }; class Shock { damage = 0; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
                    componentNames[] = {};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
                class LeftLeg 
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 0; }; class Shock { damage = 0; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
                    componentNames[] = {};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
                class RightLeg 
				{
					class Health { hitpoints = 100; transferToGlobalCoef = 0; };
					class Shock { hitpoints = 150; transferToGlobalCoef = 0; };
					class Blood { hitpoints = 100; transferToGlobalCoef = 0; };
					class ArmorType
					{
						class Melee { class Health { damage = 0; }; class Shock { damage = 1; }; };
						class Projectile { class Health { damage = 0; }; class Shock { damage = 0; }; };
						class FragGrenade { class Health { damage = 0; }; class Shock { damage = 0; }; };
					};
					transferToZonesNames[] = {};
                    transferToZonesCoefs[] = {};
                    componentNames[] = {};
					fatalInjuryCoef = -1;
					inventorySlots[] = {};
					inventorySlotsCoefs[] = {};
				};
            };
        };
	};
    class SurvivorMale_Base: SurvivorBase
	{
		model="\OMD_IDS_Core\Data\Models\ExtendedPlayerHitboxes.p3d";
	};
	class SurvivorFemale_Base: SurvivorBase
	{
		model="\OMD_IDS_Core\Data\Models\ExtendedPlayerHitboxes.p3d";
	};
};

#pragma once

// structures.h

namespace m_ue
{
	struct WeaponInformation
	{
		int32_t ammo_count;
		BYTE tier;
		std::string weapon_name;
	};

	struct APlayerInformation
	{
		AFortPlayerPawnAthena* FortnitePawn;
		USkeletalMeshComponent* Mesh;
		UObject* BoneArray;
		bool IsBot;
		int32_t TeamID;
		WeaponInformation WeaponInfo;
		std::string WeaponName;
		APlayerState* PlayerState;

		FVector BaseBonePosition;
	};


	enum bones : int
	{
		HumanBase = 0,
		HumanPelvis = 2,
		HumanLThigh1 = 71,
		HumanLThigh2 = 77,
		HumanLThigh3 = 72,
		HumanLCalf = 74,
		HumanLFoot = 86,
		HumanLToe = 76,
		HumanRThigh1 = 78,
		HumanRThigh2 = 84,
		HumanRThigh3 = 79,
		HumanRCalf = 81,
		HumanRFoot = 87,
		HumanRToe = 83,
		HumanSpine1 = 7,
		HumanSpine2 = 5,
		HumanSpine3 = 2,
		HumanLCollarbone = 9,
		HumanLUpperarm = 35,
		HumanLForearm1 = 36,
		HumanLForearm23 = 10,
		HumanLForearm2 = 34,
		HumanLForearm3 = 33,
		HumanLPalm = 32,
		HumanRCollarbone = 98,
		HumanRUpperarm = 64,
		HumanRForearm1 = 65,
		HumanRForearm23 = 39,
		HumanRForearm2 = 63,
		HumanRForearm3 = 62,
		HumanRPalm = 58,
		HumanNeck = 67,
		HumanHead = 106,
		HumanChest = 66
	};

	namespace bounds
	{
		struct bounds_t
		{
			float left, right, top, bottom;
		};

		struct bone_t
		{
			Vector2 screen;
			int index;
			bool on_screen;
			bool head_bone = false;
		};

		static std::array<bone_t, 26> BonesArray = {
		bone_t{ Vector2{}, bones::HumanHead, false, true }, bone_t{ Vector2{}, bones::HumanNeck, false },
		bone_t{ Vector2{}, bones::HumanSpine3, false },  bone_t{ Vector2{}, bones::HumanSpine2, false },
		bone_t{ Vector2{}, bones::HumanSpine1, false },  bone_t{ Vector2{}, bones::HumanPelvis, false },
		bone_t{ Vector2{}, bones::HumanLToe, false },  bone_t{ Vector2{}, bones::HumanLFoot, false },
		bone_t{ Vector2{}, bones::HumanLCalf, false },  bone_t{ Vector2{}, bones::HumanLThigh2, false },
		bone_t{ Vector2{}, bones::HumanLThigh1, false },  bone_t{ Vector2{}, bones::HumanPelvis, false },
		bone_t{ Vector2{}, bones::HumanRThigh1, false },  bone_t{ Vector2{}, bones::HumanRThigh2, false },
		bone_t{ Vector2{}, bones::HumanRCalf, false },  bone_t{ Vector2{}, bones::HumanRFoot, false },
		bone_t{ Vector2{}, bones::HumanRToe, false },  bone_t{ Vector2{}, bones::HumanLPalm, false },
		bone_t{ Vector2{}, bones::HumanLForearm1, false },  bone_t{ Vector2{}, bones::HumanLUpperarm, false },
		bone_t{ Vector2{}, bones::HumanRUpperarm, false }, bone_t{ Vector2{}, bones::HumanLForearm23, false },
		bone_t{ Vector2{}, bones::HumanLForearm2, false },
		bone_t{ Vector2{}, bones::HumanLForearm3, false },
		bone_t{ Vector2{}, bones::HumanRForearm2, false },
		bone_t{ Vector2{}, bones::HumanRForearm23, false }
		};
	}
}
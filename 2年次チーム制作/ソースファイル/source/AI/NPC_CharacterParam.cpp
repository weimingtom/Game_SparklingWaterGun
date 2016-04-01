#include	"..\All.h"


NPCParam::NPCParam() : fov(-1.0f), sight(-1.0f),
m_chara_SightInterval(-1), ammo_Interval(-1), sound_Interval(-1)
{}

bool	NPCParam::Load(char* filename)
{
	textLoader loader(filename);
	if (!loader.Be_Init())
		return false;
	loader.In();
	char buffer[64];
	for (;;)
	{
		loader._Loadstring(buffer);
		if (strcmp(buffer, "}" ) ==0)
			break;
		
		if (strcmp(buffer, "SIGHT") == 0)
		{
			sight = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "FOV") == 0)
		{
			fov = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "SIGHT_INTERVAL") == 0)
		{
			m_chara_SightInterval = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "SIGHT_INTERVAL_MAP") == 0)
		{
			m_map_SightInterval = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "AMMO_INTERVAL") == 0)
		{
			ammo_Interval = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "SOUND_INTERVAL") == 0)
		{
			sound_Interval = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "SIGHT_GUARANTEE") == 0)
		{
			sight_Guarantee = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "MAX_HEARING_RANGE") == 0)
		{
			max_HearingRange = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "REACTION_FOOTSOUND") == 0)
		{
			reaction_FootSound = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "REACTION_GUNSOUND") == 0)
		{
			reaction_GunSound = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "MEMORY_RELIABILITY_CHARACTER") == 0)
		{
			record_character_ReliabilityTime = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "MEMORY_RELIABILITY_MAPOBJECT") == 0)
		{
			record_mapobject_ReliabilityTime = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "MEMORY_RELIABILITY_EXTACT") == 0)
		{
			record_ExtAct_ReliabilityTime = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "FEEL_FEW_WEAPONCHARGE") == 0)
		{
			feel_few_WeaponCharge = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "FEEL_ATTACK") == 0)
		{
			m_feelAttack = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "FEEL_HIGH_SHREPOINT") == 0)
		{
			m_feel_high_SharePoint= loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "MAX_ROTATE_SPEED") == 0)
		{
			m_max_rotateAngle_Speed = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "ENEMY_ATTACKME_FOV") == 0)
		{
			m_enemy_AttackMe_Fov = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "FEEL_NEAR_RANGE_ATTACK") == 0)
		{
			m_feel_NearRangeDist = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "INTERVAL_CHARACTERTARGET_SEARCHPATH") == 0)
		{
			m_interval_characterTarget_SearchPath = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "INTERVAL_ADVERTISEMENT_SEARCHPATH") == 0)
		{
			m_interval_advertisementpoint_SearchPath = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "FAR_OF_VESSEL") == 0)
		{
			m_far_of_Vessel = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "PROBABILITY_GRENEDE_ATTACK") == 0)
		{
			m_probability_grenede_attack = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "RESPONSE_SIGHT") == 0)
		{
			m_response_Sight = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "RESPONSE_ATTACKME") == 0)
		{
			m_responce_AttackMe = loader.LoadInt();
			continue;
		}
		if (strcmp(buffer, "EVALUTOR_KNOCKDOWN_ENEMY_DISTBIAS_BEGIN") == 0)
		{
			m_knockdownEnemy_distBias_Begin = loader.LoadFloat();
			continue;

		}
		if (strcmp(buffer, "EVALUTOR_KNOCKDOWN_ENEMY_DISTBIAS_END") == 0)
		{
			m_knockdownEnemy_distBias_End = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "EVALUTOR_MESSAGEBIAS_MEDICALFRIEND") == 0)
		{
			m_friendMessageBias_MedicalFriend = loader.LoadFloat();
			continue;
		}
		if (strcmp(buffer, "RESPONCE_FRIENDMESSAGE") == 0)
		{
			m_responce_FriendMessage = loader.LoadInt();
			continue;
		}
	}
	return true;
}

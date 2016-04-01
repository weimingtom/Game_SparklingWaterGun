#include	"..\All.h"

#pragma region BASE_AIM

NPC_Aim::NPC_Aim() : 
m_progress_Frame(0), m_max_AimSpeed(0.05f), m_update_Interval(20)
{

}

void	NPC_Aim::Activate()
{
	m_progress_Frame = 0;
}

void	NPC_Aim::Update(NPC_Brain* brain, Character* character)
{
	if ((m_progress_Frame % m_update_Interval) == 0)
	{
		//printf("更新\n");
		Aim(brain, character);
	}
	m_progress_Frame++;

	CharacterMoveStateMessageFunc::SendRotateViewPosMsg(character, m_current_AimPos, m_max_AimSpeed);
	MyDebugMgrStr(dword_color::red, "AimPos.x  %2.2f: y %2.2f : z %2.2f \n", m_current_AimPos.x, m_current_AimPos.y, m_current_AimPos.z);
}

#pragma endregion

#pragma region WEAK_AIM

const char*	NPC_Aim_Weak::m_fuzzy_key_Dist = "Pos";
const char* NPC_Aim_Weak::m_fuzzy_key_speed = "Speed";
const char* NPC_Aim_Weak::m_fuzzy_key_DesirableTriangle = "DesirableTriangle";

NPC_Aim_Weak::NPC_Aim_Weak(char* param_Filename) :m_aim_Fuzzy(nullptr)
{
}

NPC_Aim_Weak::~NPC_Aim_Weak()
{
	SAFE_DELETE(m_aim_Fuzzy);
}

void	NPC_Aim_Weak::LoadParam(char* filename)
{
	auto Load = [](Desirable* out, textLoader* loader_ptr)
	{
		loader_ptr->In();
		loader_ptr->Search("MIN");
		out->m_not_Desirable.left = loader_ptr->LoadFloat();
		out->m_not_Desirable.midlle = loader_ptr->LoadFloat();
		out->m_not_Desirable.right = loader_ptr->LoadFloat();

		loader_ptr->Search("MIDLLE");
		out->m_desirable.left = loader_ptr->LoadFloat();
		out->m_desirable.midlle = loader_ptr->LoadFloat();
		out->m_desirable.right = loader_ptr->LoadFloat();

		loader_ptr->Search("LARGE");
		out->m_very_Desirable.left = loader_ptr->LoadFloat();
		out->m_very_Desirable.midlle = loader_ptr->LoadFloat();
		out->m_very_Desirable.right = loader_ptr->LoadFloat();

		loader_ptr->Search("}");
	};
	
	textLoader loader(filename);
#if NPC_ASSART
	MyAssert(loader.Be_Init(), "AIMFuzzyの読み込みに失敗%s", filename);
#endif
	loader.In();
	char buffer[64];
	for (;;)
	{
		loader._Loadstring(buffer);
		if (strcmp(buffer, "}") == 0)
			break;
		if (strcmp(buffer, "ANGLE") == 0)
		{
			Load(&m_pos_Angle, &loader);
		}
		if (strcmp(buffer, "VELOCITY") == 0)
		{
			Load(&m_target_Velocity, &loader);
		}
		if (strcmp(buffer, "DESIRABLE_TRIANGLE") == 0)
		{
			Load(&m_desirable_TriangleParam, &loader);
		}
	}

}

void	NPC_Aim_Weak::Initialize_FuzzyTool()
{

}

Vector3 NPC_Aim_Weak::Calculate_AimPos(NPC_Brain* brain, Character* character, const Vector3& target_Pos)
{
	Vector3 ret = target_Pos;
	ret.y += 0.3f;
	return ret;
}

void	NPC_Aim_Weak::Aim(NPC_Brain* brain, Character* character)
{
	Vector3 target_Pos,target_Velocity;
	ENTITY_ID current_Target = Accesor_Brain::Current_Target(brain);
	
	if (IS_CONTROLLOBJ_ID(current_Target))
	{
		float reliability;
		Character_Record* target_r = brain->Get_CharacterMemory(current_Target, &reliability);
#if NPC_ASSART
		MyAssert(target_r, "ターゲットがいないよ");
#endif
		target_Pos = target_r->m_sensed_Pos;
		target_Velocity = target_r->m_sensed_Velocity;
	}
	else
	{
		float reliability;
		MapObject_Record* target_r = brain->Get_MapObjectMemory(current_Target, &reliability);
#if NPC_ASSART
		MyAssert(target_r, "ターゲットがいないよ");
#endif
		target_Pos = target_r->m_sensed_Pos;
		target_Velocity = target_r->m_sensed_Velocity;
	}
	Vector3 aim_Pos = Calculate_AimPos(brain, character, target_Pos);
	m_current_AimPos = aim_Pos;
}

#pragma endregion

#pragma region NORMAL_AIM

NPC_Aim_Normal::NPC_Aim_Normal(char* param_filename)
{
	m_max_AimSpeed = 0.08f;
	m_update_Interval = 15;
}

void	NPC_Aim_Normal::Aim(NPC_Brain* brain, Character* character)
{
	Vector3 target_Pos, target_Velocity;
	ENTITY_ID current_Target = Accesor_Brain::Current_Target(brain);

	if (IS_CONTROLLOBJ_ID(current_Target))
	{
		float reliability;
		Character_Record* target_r = brain->Get_CharacterMemory(current_Target, &reliability);
#if NPC_ASSART
		MyAssert(target_r, "ターゲットがいないよ");
#endif
		target_Pos = target_r->m_sensed_Pos;
		target_Velocity = target_r->m_sensed_Velocity;
	}
	else
	{
		float reliability;
		MapObject_Record* target_r = brain->Get_MapObjectMemory(current_Target, &reliability);
#if NPC_ASSART
		MyAssert(target_r, "ターゲットがいないよ");
#endif
		target_Pos = target_r->m_sensed_Pos;
		target_Velocity = target_r->m_sensed_Velocity;
	}
	Vector3 aim_Pos = Calculate_AimPos(brain, character, target_Pos,target_Velocity);
	m_current_AimPos = aim_Pos;
}

Vector3 NPC_Aim_Normal::Calculate_AimPos(NPC_Brain* brain, Character* character,
	const Vector3& target_Pos, const Vector3& target_Velocity)
{
	//少し先のほうにする
	Vector3 target = target_Pos + target_Velocity *1.5f;
	//総体的な右方向の速度を取得
//	Vector3 my_Right = ChrFunc::GetRight(character);
	//my_Right.Normalize();
	Vector3 my_Velocity = ChrFunc::GetParam(character).m_move;
	//float projection_cos = Vector3Dot(my_Velocity, my_Right);

	target = target - my_Velocity * 2.5f;
	

	target.y += 0.3f;
	return target;

}

#pragma endregion

#pragma region STRONG

void	NPC_Aim_Strong::Aim(NPC_Brain* brain, Character* character)
{
	
}

Vector3 NPC_Aim_Strong::Calculate_AimPos(NPC_Brain* brain, Character* character,
	const Vector3& target_Pos, const Vector3& target_Velocity)
{
	return Vector3(0, 0, 0);
}

#pragma endregion

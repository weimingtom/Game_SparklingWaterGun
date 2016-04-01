#include	"All.h"

bool	Goal::Put_CurrentBehavior(NPC_Brain* brain, Character* character)
{
	//���݂̍s�����B���ł����ꍇ�U�镑�����i�߂�
	behavior_List[current_Behavior]->Exit(brain, character);
	current_Behavior++;

	//�Ō�܂ł�����
	if (current_Behavior == num_Behavior)
	{
		return true;
	}
	//�G���[�`�F�b�N
	if (!behavior_List[current_Behavior]){
		console::Print("���̃L�����N�^�[�̐U�镑��������܂���");
		console::Print(brain->character_Name);
		console::Print(goal_Name);
		return false;
	}

	//�G�b�W�ړ��Ȃ�p�X������\������
	if (behavior_List[current_Behavior]->tag == behavior_tag::move_edge)
	{
		WAIT_PATHSEARCH_OBJ send_CharacterInfo;
		send_CharacterInfo.brain = brain;
		send_CharacterInfo.character = character;
		AI_MESSAGE msg(ai_messagetype::demand_search_path,&send_CharacterInfo);
		field_Info->HandleMessage(&msg);
		
	}
	behavior_List[current_Behavior]->Enter(brain, character);
	//�܂��U�镑�����c���Ă���ꍇ��false
	return false;
}

goal_status Goal::Behavior(NPC_Brain* brain, Character* character)
{
	goal_status ret = goal_status::active;


	if (SAFE_EXE(behavior_List[current_Behavior], Execute, brain, character))
	{
		//�s����i�߂�
		if (Put_CurrentBehavior(brain, character))
		{
			//�U�镑���񂪍Ō�܂ŗ����炱�̃S�[���͒B���������ƂɂȂ�
			ret = goal_status::completed;
		}
	}
	else
	{
		int a = 0;
	}
	return ret;
}

bool	Goal::Path_Research(NPC_Brain* brain, Character* character, Vector3& goal_Pos)
{
	Astar_Standard search;
	Field_Node* goal_Node = field_Info->Find_Node_Pos(goal_Pos);
	search.Set_GoalNode(goal_Node);
	Field_Node* start_Node = field_Info->Find_Node_Pos(ControllObjFunc::GetPos(character));
	search.Set_StartNode(start_Node);

	AI_MESSAGE msg(ai_messagetype::search_path_ok, static_cast<void*>(&search));
	behavior_List[current_Behavior]->HandleMessage(&msg);

	//�S�[�����W���`����
	msg.type = ai_messagetype::set_currentGoalPos;
	msg.extra_Info = &goal_Pos;
	behavior_List[current_Behavior]->HandleMessage(&msg);
	field_Debug->Send_Goal(character->GetId(), search.dicision_Goal);
	return false;
}
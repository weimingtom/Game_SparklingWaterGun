#include	"All.h"
#include "StartCountDown.h"
#include "SceneCutWater.h"
#include <fstream>
//*****************************************************************************************************************************
//
//	�O���[�o���ϐ�
//
//*****************************************************************************************************************************

//*****************************************************************************************************************************
//
//	������
//
//*****************************************************************************************************************************

sceneMain::sceneMain(CODMatchData* match_data_ptr)
{
	m_pMatch_data_ptr = new CODMatchData;
	*m_pMatch_data_ptr = *match_data_ptr;
}


bool sceneMain::Initialize()
{
	//�}�b�`�f�[�^���쐬
	{
		switch (m_pMatch_data_ptr->player_num)
		{
		case 2:
			m_pMatch_data_ptr->computer_num = 4;
			break;
		case 3:
			m_pMatch_data_ptr->computer_num = 3;
			break;
		case 4:
			m_pMatch_data_ptr->computer_num = 2;
			break;
		default:
			m_pMatch_data_ptr->computer_num = 7;
			break;
		}

		//�f�o�b�O�p�}�b�`�f�[�^���쐬
		
		m_pMatch_data_ptr->all_player_num = m_pMatch_data_ptr->player_num + m_pMatch_data_ptr->computer_num;

		m_pMatch_data_ptr->match_type = game_match_type::FLAG_GAME;
		m_pMatch_data_ptr->pMatch_state = new ShareMatchState;
	}

	if (m_pMatch_data_ptr->player_num > 1)
	{
		MainFrame->SetFPSMode(FPS_30);
	}


	//�}�b�`���쐬
	m_pCODBO_Manager = new ColaOfDessensionManager(m_pMatch_data_ptr);
	m_pCODBO_Manager->Initialize();

	return true;
}

//*****************************************************************************************************************************
//
//		���
//
//*****************************************************************************************************************************

sceneMain::~sceneMain()
{
	delete m_pCODBO_Manager;
	delete m_pMatch_data_ptr;
}

//*****************************************************************************************************************************
//
//		�X�V
//
//*****************************************************************************************************************************
void	sceneMain::Update()
{
	Scene* pNextScene = nullptr;

	m_pCODBO_Manager->Update(); 
//	m_pddddd->Update(8);

	if (m_pCODBO_Manager->isResult(&pNextScene))
	{
		MainFrame->ChangeScene(pNextScene);
	}
	
}

//*****************************************************************************************************************************
//
//		�`��֘A
//
//*****************************************************************************************************************************

void	sceneMain::Render()
{
	m_pCODBO_Manager->Render();
}




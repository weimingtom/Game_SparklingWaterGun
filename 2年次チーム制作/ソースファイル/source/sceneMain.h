#ifndef __SCENE_MAIN_H__
#define __SCENE_MAIN_H__

class ColaOfDessensionManager;
class SceneCutWater;
struct CODMatchData;
//*****************************************************************************************************************************
//
//		���C���V�[��
//
//*****************************************************************************************************************************

class	sceneMain : public Scene
{
private:
	CODMatchData*				m_pMatch_data_ptr;
	ColaOfDessensionManager*	m_pCODBO_Manager;
public:
	sceneMain(CODMatchData* match_data_ptr);
	~sceneMain();
	//	������
	bool Initialize();
	//	�X�V�E�`��
	void Update();	//	�X�V
	void Render();	//	�`��
};


#endif
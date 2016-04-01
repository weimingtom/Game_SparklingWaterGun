#ifndef __SCENE_MAIN_H__
#define __SCENE_MAIN_H__

class ColaOfDessensionManager;
class SceneCutWater;
struct CODMatchData;
//*****************************************************************************************************************************
//
//		メインシーン
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
	//	初期化
	bool Initialize();
	//	更新・描画
	void Update();	//	更新
	void Render();	//	描画
};


#endif
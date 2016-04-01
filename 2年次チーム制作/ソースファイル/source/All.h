#ifndef __ALL_H__
#define __ALL_H__

//****************************************************//
//　全ヘッダをインクルードするヘッダ
//****************************************************//
#define LIGHTMAP FALSE
#define PARACHUTE_RESPORN FALSE

//iex
#include "iextreme.h"
#include "textLoader.h"
#include "system\Framework.h"	
#include "system\Scene.h"
#include "system\Scene.h"
#include "sceneMain.h"
#include "SceneSelect.h"
#include "SceneEnd.h"

//debug
#include "ItDebug.h"
#include "itDebugStringManager.h"
#include "DebugParticle.h"
#include "console\MyConsole.h"
#include "console\WindowClass.h"

//utillity_header
#include "ColorUtility.h"
#include "ppSpatialPartitionning.h"
#include "StateMachine\StateTemplate.h"
#include "Collision.h"
#include "MeshManager.h"
#include "GlobalTexture.h"

//effect
#include "PolygonEffect\EffectClass.h"
#include "EffectResource.h"
#include "ScreenPositionEffect.h"

//lua
#include "LuaScript\LuaManager.h"
#include "LuaLoadParam.h"
#include "LuaKeyDefine.h"

//game_system
#include "GameSystem\GameBase.h"
#include "GameSystem\GameIDDefine.h"
#include "GameSystem\GameIDManager.h"
#include "GameSystem\GameMessageType.h"
#include "GameSystem\GameController.h"
#include "GameSound\GameSound.h"
#include "CODPlaySound.h"

#include "CODBO.h"
#include "GameMatchInfomation.h"
#include "CODBO_DebugMatch.h"//test
#include "CODBO_FlagMatch.h"
#include "CODBO_ShareMatch.h"

#include "Camera.h"
#include "CameraMove.h"
#include "TriggerManager.h"
#include "DamageHit.h"
#include "ScreenSeparation.h"
#include "screen\ScreenCreater.h"
#include "TimeRender.h"
#include "ScoreCalcManager.h"
#include "ShareIcon.h"
#include "PowerUpIcon.h"
#include "NewsTelopUpdater.h"

//RenderInterface
#include	"screen\\RenderInterFace.h"


//map
#include "map/MapObject.h"
#include "map/ObjectAimTarget.h"
#include "map/ShareRatioObject.h"
#include "map\Crane.h"

//bullet
#include "ColaBullet.h"
#include "ColaWater.h"
#include "AimPoint.h"
#include "WaterPool.h"
#include "ColaGauge.h"
#include "TeamViewData.h"
#include "EmptyCan.h"

#include "Controller.h"
#include "ControllerManager.h"

#include "ControllObject.h"
#include "ControllObjectFunction.h"
#include "ControllObjectManager.h"

//character
#include "Character.h"
#include "CharacterWepon.h"
#include "CharacterUtilityFunction.h"
#include "CharacterMoveFunction.h"
#include "CharacterMeshManager.h"
#include "CharacterBrain.h"
#include "CharacterBrainRealPlayer.h"
#include "CharacterMoveState.h"
#include "CharacterMoveStateMessageFunc.h"
#include "CharacterAim.h"
#include "CharacterUI.h"
#include "Grenade.h"

#include "TestCharacterModel.h"//test
#include "CharacterModelResource.h"

//resporn
#include "RespornControllobj.h"
#include "StartControllobj.h"

//movie
#include "MovieControllobj.h"

//test
#include "AI/CP11_Rand.h"
#include	"Console.h"
//Fuzzy
namespace dword_color
{
	const DWORD white = 0xFFFFFFFF;
	const DWORD black = 0xFF000000;
	const DWORD red = 0xFFFF0000;
	const DWORD blue = 0xFF0000FF;
	const DWORD yellow = 0xFFFFFF00;
	const DWORD green = 0xFF00FF00;
}

#include "AI/Tfunctions.h"
#include "AI/Fuzzy_h.h"

//AI_system
#include	"RenderLine.h"

#include	"AI/NPC_Enum.h"
#include	"AI/Environmental_Sound.h"
#include	"AI/Field_Division.h"
#include	"AI/Behavior_ProcessFunction.h"
#include	"AI/SoundMessage.h"
#include	"AI/NPC_Behavior.h"
#include	"AI/NPC_Record.h"


#include	"AI/MemoryFunction.h"
#include	"AI/NPC_WorkingMemory.h"
#include	"AI/NPC_Predicate.h"
#include	"AI/NPC_Symbol.h"
#include	"AI/NPC_Goal.h"
#include	"AI/PathPlanner.h"
#include	"AI/Navigation_System.h"

#include	"AI/NPC_PathSearchAlg.h"
#include	"AI/NPC_GoalEvalutor.h"
#include	"AI\NPC_FriendMessage.h"
#include	"AI/NPC_Brain.h"
#include	"AI\NPC_Commander.h"
#include	"AI/NPC_Censor.h"
#include	"AI/NPC_MessageDispatcher.h"
#include	"AI/NPC_Aim.h"
#include	"AI/NPC_Factory.h"

#include	"AI/Environmental_Sound.h"
#include	"BillBoard.h"


//Shader
#include	"ShadowMap.h"
#include	"screen\Radiation_Blur.h"
#include	"screen\Screen_Distortion.h"

#endif

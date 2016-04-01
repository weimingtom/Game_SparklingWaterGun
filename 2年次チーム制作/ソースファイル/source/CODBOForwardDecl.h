#ifndef __CODBO_FORWARD_DECL_H__
#define __CODBO_FORWARD_DECL_H__

//*******************************************
//	ëOï˚êÈåæÉwÉbÉ_
//*******************************************
#include <list>



class Camera;
class ControllerManager;
class BulletManager;
class DamageObjectManager;
class AimTargetManager;
class ControllObject;
class Scene;

class Controller;
typedef Controller* LPCONTROLLER;

typedef Camera* LPCAMERA;
typedef const LPCAMERA LPCCAMERA;

struct CODMatchData;
typedef CODMatchData *LPCODMatchData;

struct ControllerButtonSet;
typedef const ControllerButtonSet*  LPCONTROLLERBUTTONSET;

struct PlayerInfo;
typedef PlayerInfo* LPPlayerInfo;

class	Character;
typedef Character	*LPCHARACTER, *LPCharacter;

class	CharacterModelBase;
typedef CharacterModelBase* LPCHARACTERMODELBASE;

class CharacterBrainBase;
typedef CharacterBrainBase*	LPCHARACTER_BRAINBASE;

class CharacterMoveStateBase;
typedef CharacterMoveStateBase* LPCHARACTERMOVESTATEBASE;

class CharacterMoveStateTable;
typedef CharacterMoveStateTable* LPCHARACTERMOVESTATETABLE;

class MapObjectInterface;
typedef MapObjectInterface* LPMapObjectInterface;

class CharacterUI;
typedef CharacterUI* LPCHARACTER_UI;

class CharacterWeponBase;
typedef CharacterWeponBase* LPCHARACTERWEPONBASE;

class WaterPoolManager;
typedef WaterPoolManager* LPWATERPOOLMANAGER;

typedef ControllObject CONTROLLOBJECT;
typedef CONTROLLOBJECT* LPCONTROLLOBJCT;

class GameMessage;
typedef GameMessage* LPGAMEMESSAGE;

typedef std::list<LPCONTROLLOBJCT> CONTROLLOBJLIST;

class ColaGauge;
typedef ColaGauge* LPCOLAGAUGE;


class PositionEffect;
typedef PositionEffect* LPPOSITION_EFFECT;

class CharacterLockOn;
typedef CharacterLockOn* LPCHARACTER_LOCKON;

class ColaShareGauge;
typedef ColaShareGauge* LPCOLASHAREGAUGE;

class DamageObject;

typedef int TEAM_TYPE;

class TimeRender;
typedef TimeRender* LPTIMERRENDER;

class TeamViewData;
typedef TeamViewData* LPTEAMVIEWDATA;

class TeamDataManager;
typedef TeamDataManager* LPTEAMDATAMGR;

class R_InterFace_Character;

class StartCountDown;
typedef StartCountDown* LPSTARTCOUNTDOWN;

class ScoreCalcManager;
typedef ScoreCalcManager* LPSCORECALCMGR;

struct TeamData;
typedef TeamData* LPTEAMDATA;

class ScreenIconManager;
typedef ScreenIconManager* LPSCRICONMGR;

class NewsTelopManager;
typedef NewsTelopManager* LPNEWSTELOPMGR;

class NewsTelopUpdaterShareMatch;

class GameSoundManager;

#endif
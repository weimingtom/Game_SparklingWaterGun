#ifndef __CHARACTER_MOVE_STATE_MESSAGE_FUNC_H__
#define __CHARACTER_MOVE_STATE_MESSAGE_FUNC_H__

#include "Character.h"
#include "FriendMessage.h"

//*************************************************************
//	キャラクタへの移動命令メッセージを送るクラス
//*************************************************************

class CharacterMoveStateMessageFunc;
typedef CharacterMoveStateMessageFunc ChrStateMsg;


class CharacterMoveStateMessageFunc
{
private:
	CharacterMoveStateMessageFunc();
	~CharacterMoveStateMessageFunc();
private:
	static void SendViewMsg(LPCHARACTER p, CRVector3 view_vec);
public:
	static void SendChangeStateMsg(LPCHARACTER p, CHARACTER_STATE_TYPE set_state_type);		//指定したステートに変更するメッセージを送る(適切なステート出なかった場合無視される)
	static void SendMoveVecMsg(LPCHARACTER p, CRVector3 move_vec);							//移動ベクトルを格納したメッセージを作成し、送信する
	static void SendRotateViewPosMsg(LPCHARACTER p,CRVector3 view_pos,RADIAN rotate_speed);	//指定した場所に向かって視線をspeed分向ける
	static void SendRotateYViewMsg(LPCHARACTER p, RADIAN rotate_speed);						//キャラクタをY軸にrotate_speed分だけ回転させる
	static void SendRotateXViewMsg(LPCHARACTER p, RADIAN rotate_speed);						//キャラクタをX軸にrotate_speed分だけ回転させる
public:
	static void SendShuffleMsg(LPCHARACTER p, bool shuffle);	//shuffleするかどうかのメッセーじを送る
	static void SendWeponFireMsg(LPCHARACTER p);											//現在の武器を発射するメッセージを送る
	static void SendLockOnMsg(LPCHARACTER p, bool lockOn);									//ロックオンするかどうかのメッセージを送る
	static void SendChangeCameraPosMsg(LPCHARACTER p);									//ロックオン状態での左右切り替えメッセージを送る
	static void SendJumpHoverMsg(LPCHARACTER p);
public:
	static void SendFriendMsgIcon(LPCHARACTER p, FriendMessageIcon::MsgType type);
};

#endif
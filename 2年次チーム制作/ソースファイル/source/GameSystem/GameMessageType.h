#ifndef __GAME_MESSAGE_TYPE_H__
#define __GAME_MESSAGE_TYPE_H__

//**********************************************************
//	ゲーム中で使われるメッセージを列挙
//**********************************************************

namespace msg_t
{
	enum _ENUM_MSG_TYPE	//←これよりも下でtypedefしているほうを使ってほしい…
	{
		error_msg,	//エラータイプ

		//環境音メッセージ
		env_sound,
		memory_update,
		//キャラクタステートテーブルに対してステートを変更するメッセージ(exinfoとしてCHARACTER_STATE_TYPE型のポインタを設定すると中で変更する)
		chr_table_state_change,
		//キャラクタステートテーブルに対して変更可能であればステートを変更するメッセージ
		chr_table_state_change_low_lvl,
		//キャラクタの現在実行しているステートに対するメッセージ(exinfoとしてCharacterStateMessageのポインタを設定すると中で処理する)
		to_chr_state,
		//キャラクタに対し、仲間へのメッセージアイコンを表示してほしいときにおくるメッセージ(exinfoとしてFriendMessageIcon::MsgType型のポインタを設定すると中で処理する)
		chr_friend_msg_icon,
		//味方全員の脳に対して、メッセージアイコンを表示したときに送るメッセージ(exinfoとしてFriendIconMsgInfoのポインタを送る)
		apper_friend_msg_icon,
		//攻撃があたった時にcharacterに対して送るメッセージ
		hit_my_attack,
		//キャラクタの能力値をUPさせる(exinfoとして値(float)を送る)
		up_character_move_speed,
		up_character_charge_speed,
		//バレットマネージャに対するメッセージ(exinfoとしてBulletBase型を継承したクラスポインタを設定すると弾リストに追加する)
		add_bullet,

		//MapObjectInterfaceに対して、乗った時の衝撃を送るメッセージ(exinfoとしてControllObj型のポインタを送る)
		ride_obj,
		
		//敵を倒した時のスコア加算メッセージ(exinfoとしてAddScoreMsgDataを送る)
		add_score_msg,
		//スコア描画メッセージ(exinfoとしてTEAM_TYPEを送る)
		render_score_msg,
		//アイコン追加メッセージ
		send_screen_icon_msg,
		//テロップマネージャに対し、テロップ追加メッセージ(exinfoとしてNewsTelopBase型を送る)
		send_news_telop,

		//アイコン描画メッセージ(exinfoとしてRenderOwnerMsgExinfoを送る)
		news_telop_render_msg,

		//アイコン描画メッセージ(exinfoとしてRenderOwnerMsgExinfoを送る)
		icon_render_msg,

		//時間描画メッセージ(exinfoとしてTimerRenderMsgInfoを送る)
		timer_render_msg,

		//SE再生メッセージ(exinfoとしてcod_sound::PlaySEParamを送る)
		play_se_msg,

		//SE再生メッセージ(exinfoとしてbgm_typeを送る)
		play_bgm_msg,

		//プランニング
		replanning,

		//脳に対してダメージを受けたときに送るメッセージ(exinfoとしてDamageParam型を送る)
		damage_hit,
	};
}

typedef msg_t::_ENUM_MSG_TYPE		MSG_TYPE;	//ゲーム内メッセージタイプ



#endif
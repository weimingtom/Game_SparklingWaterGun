#ifndef __CHARACTER_EYE_H__
#define __CHARACTER_EYE_H__

#include "iextreme.h"

//キャラクタの目クラス

class CharacterEye
{
public:
	//目の状態変数
	enum EYE_STATE
	{
		_OPEN = 0,	//通常
		_CLOSE,		//瞬き
		_DAMAGE,	//被ダメージ時

		__STATE_MAX,
	};
private:
	EYE_STATE		m_State;					//現在の状態
	int				m_Count;					//状態の継続時間
	float			m_Scale;					//目の大きさ
	LPIEX2DOBJ		m_pTextures[__STATE_MAX];	//目のテクスチャ
	LVERTEX			m_Vertexs[4];				//描画用目の頂点
public:
	CharacterEye(
		float			Scale	//目の大きさ
		);
	~CharacterEye();
private:
	void UpdateOpenEye();
	void UpdateCloseEye();
	void UpdateDamageEye();
public:
	//目のテクスチャをセットする
	void SetEyeTexture(
		EYE_STATE		set_eye_state,	//設定する目のタイプ
		LPIEX2DOBJ		pTexture		//テクスチャへのポインタ
		);

	//毎フレームの更新
	void Update();

	//目の状態を変更
	void SetState(
		EYE_STATE next		//次の状態
		);

	//描画
	void Render(
		CRVector3 right_eye,	//右目座標
		CRVector3 left_eye,		//左目座標
		CRVector3 front,		//顔の向きベクトル(正規化済みであること)
		CRVector3 right,		//顔のよこベクトル(正規化済みであること)
		CRVector3 up			//顔のうえベクトル(正規化済みであること)
		);
};

#endif
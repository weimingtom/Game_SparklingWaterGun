#include "CharacterEye.h"



CharacterEye::CharacterEye(
	float			Scale					//目の大きさ
	):
	m_Scale(Scale),
	m_Count(0),
	m_State(_OPEN)
{
	//初期化
	for (int i = 0; i < __STATE_MAX; ++i)
	{
		m_pTextures[i] = nullptr;
	}
}



CharacterEye::~CharacterEye()
{

}


void CharacterEye::UpdateOpenEye()
{
	//乱数で瞬き状態に移行
	if (rand() % 20 == 0)
	{
		SetState(_CLOSE);
	}
}


void CharacterEye::UpdateCloseEye()
{
	//一定時間で通常状態に
	if (m_Count++ > 5)
	{
		SetState(_OPEN);
	}
}

void CharacterEye::UpdateDamageEye()
{
	//何もしない

}



void CharacterEye::Update()
{
	//毎フレームの更新(目の状態更新)
	
	void(CharacterEye::*functions[])()=
	{
		&CharacterEye::UpdateOpenEye,
		&CharacterEye::UpdateCloseEye,
		&CharacterEye::UpdateDamageEye
	};

	//更新
	(this->*functions[m_State])();
}

//目のテクスチャをセットする
void CharacterEye::SetEyeTexture(
	EYE_STATE		set_eye_state,	//設定する目のタイプ
	LPIEX2DOBJ		pTexture		//テクスチャへのポインタ
	)
{
	MyAssert(
		(int)set_eye_state >= 0 &&
		(int)set_eye_state < __STATE_MAX,
		"存在しない目のタイプにテクスチャが設定されました type=%d ",
		(int)set_eye_state);

	m_pTextures[set_eye_state] = pTexture;
}

//目の状態を変更
void CharacterEye::SetState(
	EYE_STATE next		//次の状態
	)
{
	m_State = next;
	m_Count = 0;
}


//描画
void CharacterEye::Render(
	CRVector3 right_eye,	//右目座標
	CRVector3 left_eye,		//左目座標
	CRVector3 front,		//顔の向きベクトル(正規化済みであること)
	CRVector3 right,		//顔のよこベクトル(正規化済みであること)
	CRVector3 up			//顔のうえベクトル(正規化済みであること)
	)
{
	LPIEX2DOBJ pTex = m_pTextures[m_State];
	Vector3	sUp(up), sFront(front), sRight(right);
	int i;

	sUp *= m_Scale;
	sFront *= m_Scale;
	sRight *= m_Scale;

	//頂点に情報を設定
	for (i = 0; i < 4; ++i)
	{
		m_Vertexs[i].color = 0xFFFFFFFF;
		m_Vertexs[i]._v = left_eye;
	}

	m_Vertexs[0].tu = 0;
	m_Vertexs[0].tv = 0;

	m_Vertexs[1].tu = 1;
	m_Vertexs[1].tv = 0;

	m_Vertexs[2].tu = 0;
	m_Vertexs[2].tv = 1;

	m_Vertexs[3].tu = 1;
	m_Vertexs[3].tv = 1;

	m_Vertexs[0]._v = -sRight + sUp + m_Vertexs[0]._v;
	m_Vertexs[1]._v = sRight + sUp + m_Vertexs[1]._v;
	m_Vertexs[2]._v = -sRight + -sUp + m_Vertexs[2]._v;
	m_Vertexs[3]._v = sRight + -sUp + m_Vertexs[3]._v;

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//描画
	iexPolygon::Render3D(m_Vertexs, 2, pTex, RS_COPY);


	//右目の情報設定
	for (i = 0; i < 4; ++i)
	{
		m_Vertexs[i].tu = 1 - m_Vertexs[i].tu;
		m_Vertexs[i]._v =
			-left_eye + m_Vertexs[i]._v + right_eye;
	}


	//描画
	iexPolygon::Render3D(m_Vertexs, 2, pTex, RS_COPY);

	iexSystem::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
#include	"SunShine.h"
#include	"GameMatchInfomation.h"
#include	"map\MapObject.h"

#define ALPHA(a)  (0x00FFFFFF|(DWORD)(a*255)<<24)


SunShine::SunShine(const Vector3& LighDirVec) :
visible(true),
dirLightvec(LighDirVec)
{
	timer = PI / 2;
	lpObj = new iex2DObj("DATA\\TEXTURE\\UI\\lensflare_neo.png");
	LensEffect Pattern[] = {
			{ 0, 0, 0, 2.8f, 0.6f*0.5f },
			{ 1, 0, 0, 0.2f, 0.8f*0.5f },
			{ 2, 0, 0, 0.8f, 0.6f*0.5f },
			{ 3, 0, 0, 1.8f, 0.8f*0.5f },
			{ 1, 0, 0, 2.0f, 0.4f*0.5f },
			{ 3, 0, 0, 1.2f, 0.6f*0.5f },
			{ 2, 0, 0, 0.6f, 0.6f*0.5f },
			{ 3, 0, 0, 2.2f, 0.8f*0.5f },
		//	{ 4, 0, 0, 2.8f, 0.4f*0.5f },
	};
	memcpy(data, Pattern, sizeof(LensEffect)*LENSEFFECT);


}

SunShine::~SunShine()
{
	delete lpObj;
}
void	SunShine::UpDate(LPCAMERA camera)
{

	//	レンズフレア処理
	Vector3 ScreenLightPos;			//	太陽の位置
	POINT	center;					//	画面中央
	//	ライトの２D変換
	visible = camera->ClientToScreen(ScreenLightPos, dirLightvec * 3000);

	POINT dir;
	RECTI	rc = GAMEMATCHINFO.GetScreenSize();

	center_len = 1 - (Vector3XYLength(ScreenLightPos) / sqrtf(2));
	center_len *= 0.25f;

	ScreenLightPos.x = (ScreenLightPos.x + 1)*0.5f*(float)rc.width;
	ScreenLightPos.y = ((ScreenLightPos.y *-0.5f)+0.5f)*(float)rc.height;

	center.x = rc.width / 2;
	center.y = rc.height / 2;

	dir.x = (LONG)(center.x - ScreenLightPos.x);
	dir.y = (LONG)(center.y - ScreenLightPos.y);

	float dist = (float)dir.x*(float)dir.x + (float)dir.y*(float)dir.y;

	LensAlpha =
		1.0f - sqrtf(dist)
		/ sqrtf((float)center.x*(float)center.x + (float)center.y*(float)center.y);

	dir.x /= (LONG)LENSEFFECT;
	dir.y /= (LONG)LENSEFFECT;


	// 2D太陽位置の設定
	data[0].pos.x = (LONG)ScreenLightPos.x;
	data[0].pos.y = (LONG)ScreenLightPos.y;


	for (int i = 1; i<LENSEFFECT; i++)
	{
		data[i].pos.x = data[0].pos.x + 2 * dir.x *i;
		data[i].pos.y = data[0].pos.y + 2 * dir.y *i;
	}

}

bool SunShine::RayCheck(CRVector3 cpos, CRVector3 vec)
{
	Vector3 out, pos(cpos), v(vec);
	float dist = 100000;

	return !MAP_MANAGER.RayPick(&out, &pos, &v, &dist);
}

//
//			描画		
//
void	SunShine::Render(LPCAMERA camera)
{
	if (!RayCheck(camera->GetPos(), dirLightvec))return;

	RECTI	rc = GAMEMATCHINFO.GetScreenSize();
	int ws, hs;
	GAMEMATCHINFO.GetNowGameScrDivision(ws, hs);

	UpDate(camera);

	if (!visible){
		timer = -PI / 4;
		return;
	}
	timer += 0.05f;
	if (timer > PI / 2) timer = PI / 2;

	for (int i = 0; i<LENSEFFECT; i++){
		int w = (int)(data[i].scale * 256);
		float alpha = 1.0f;
		if (i != 0) alpha = LensAlpha;
		lpObj->Render((int)((int)data[i].pos.x - w / 2) / ws, (int)((int)data[i].pos.y - w / 2) / hs, w / ws, w / hs,
			(data[i].type % 4) * 256,
			(data[i].type / 4) * 256,
			256, 256, RS_ADD,
			ALPHA(data[i].alpha*alpha));
	}

	float alpha = cosf(timer)*0.1f;
	alpha = max(alpha, center_len);
	alpha = min(alpha, 1);

	//	ホワイトアウト
	iexPolygon::Rect(0, 0, rc.width, rc.height, RS_ADD, ALPHA(alpha));

}

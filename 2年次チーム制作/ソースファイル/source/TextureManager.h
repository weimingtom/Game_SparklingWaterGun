#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include "iextreme.h"
#include "SingletonTemplate.h"

//*************************************************************
//メッシュ管理ヘッダ
//*************************************************************
typedef iex2DObj	MYTEXTURE, *LPMYTEXTURE;

//*************************************************************
//メッシュ管理クラス
//*************************************************************
class TextureManager
{
public:
	enum TEXTURE_ID
	{
		ERR_ID,
		NUMBER, 

		TIMER,
		SHARE_UP,
		KILL_ENEMY,
		WATER_DROP_ANIMATION,

		POWER_UP,
		SHARE_OBJ_ICON,
		FRIEND_MSG_ICON,

		COLA_GAUGE,
		COLA_GAUGE_SOUR,
		COLA_GAUGE_CLIP,

		CHR_TEXTURE_SODA,
		CHR_TEXTURE_LEMON,
		CHR_TEXTURE_MELON,
		CHR_TEXTURE_COLA,

		CHR_EYE_CLOSE,
		CHR_EYE_DAMAGE,

		CHR_EYE1,
		CHR_EYE2,
		CHR_EYE3,
		CHR_EYE4,
		CHR_EYE5,
		CHR_EYE6,

		AD_TEXTURE_SODA,
		AD_TEXTURE_LEMON,
		AD_TEXTURE_MELON,
		AD_TEXTURE_COLA,


		LOGO_TEXTURE_SODA,
		LOGO_TEXTURE_LEMON,
		LOGO_TEXTURE_MELON,
		LOGO_TEXTURE_COLA,


		VENDING_TEXTURE_SODA,
		VENDING_TEXTURE_LEMON,
		VENDING_TEXTURE_MELON,
		VENDING_TEXTURE_COLA,

		TRUCK_TEXTURE_USUAL,
		TRUCK_TEXTURE_SODA,
		TRUCK_TEXTURE_LEMON,
		TRUCK_TEXTURE_MELON,
		TRUCK_TEXTURE_COLA,

		AIRSHIP_TEXTURE_SODA,
		AIRSHIP_TEXTURE_LEMON,
		AIRSHIP_TEXTURE_MELON,
		AIRSHIP_TEXTURE_COLA,

		PET_TEXTURE_SODA,
		PET_TEXTURE_LEMON,
		PET_TEXTURE_MELON,
		PET_TEXTURE_COLA,

		__MAX
	};
private:
	const int		m_TextureMax;
	LPMYTEXTURE*	m_pTexture_array;
public:
	TextureManager();
	~TextureManager();
public:
	void GetEyeTextures(int player_num, LPIEX2DOBJ pOutTex[3]);

	LPMYTEXTURE	GetPetTexturePtr(int team_type);
	LPMYTEXTURE	GetADTexturePtr(int team_type);
	LPMYTEXTURE	GetLogoTexturePtr(int team_type);
	LPMYTEXTURE	GetVendingTexturePtr(int team_type);
	LPMYTEXTURE	GetTruckTexturePtr(int team_type);	//-1で通常ver
	LPMYTEXTURE	GetAirShipTexturePtr(int team_type);	//-1で通常ver

	bool		LoadTexture();
	TEXTURE_ID	TextureRegister(LPMYTEXTURE pTex);					
	LPMYTEXTURE	GetTexturePtr(TEXTURE_ID id);		
};

#endif
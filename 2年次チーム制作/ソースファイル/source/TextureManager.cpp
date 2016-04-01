#include "TextureManager.h"

TextureManager::TextureManager() :
m_TextureMax(500)
{
	m_pTexture_array = new LPMYTEXTURE[m_TextureMax];
	for (int i = 0; i < m_TextureMax; ++i)
		m_pTexture_array[i] = nullptr;

}

TextureManager::~TextureManager()
{
	for (int i = 0; i < m_TextureMax; ++i)
	{
	//	MyDebugString("delete texture = %d\n", i);
		delete m_pTexture_array[i];
	}
	delete[]m_pTexture_array;
}

void TextureManager::GetEyeTextures(int player_num, LPIEX2DOBJ pOutTex[3])
{
	const int n = (int)CHR_EYE1 + (player_num % 6);

	pOutTex[0] = m_pTexture_array[n];
	pOutTex[1] = m_pTexture_array[CHR_EYE_CLOSE];
	pOutTex[2] = m_pTexture_array[CHR_EYE_DAMAGE];
}

LPMYTEXTURE	TextureManager::GetPetTexturePtr(int team_type)
{
	MyAssert(team_type >= 0 && team_type < 4, "存在しないチーム番号　%d ", team_type);

	return m_pTexture_array[(int)PET_TEXTURE_SODA + team_type];
}

LPMYTEXTURE	TextureManager::GetADTexturePtr(int team_type)
{
	MyAssert(team_type >= 0 && team_type < 4, "存在しないチーム番号　%d ", team_type);

	return m_pTexture_array[(int)AD_TEXTURE_SODA + team_type];
}

LPMYTEXTURE	TextureManager::GetLogoTexturePtr(int team_type)
{
	MyAssert(team_type >= 0 && team_type < 4, "存在しないチーム番号　%d ", team_type);

	return m_pTexture_array[(int)LOGO_TEXTURE_SODA + team_type];
}

LPMYTEXTURE	TextureManager::GetVendingTexturePtr(int team_type)
{
	MyAssert(team_type >= 0 && team_type < 4, "存在しないチーム番号　%d ", team_type);

	return m_pTexture_array[(int)VENDING_TEXTURE_SODA + team_type];
}


LPMYTEXTURE	TextureManager::GetTruckTexturePtr(int team_type)
{
	if (team_type == -1)
	{
		return m_pTexture_array[(int)TRUCK_TEXTURE_USUAL];
	}
	MyAssert(team_type >= 0 && team_type < 4, "存在しないチーム番号　%d ", team_type);

	return m_pTexture_array[(int)TRUCK_TEXTURE_SODA + team_type];
}

LPMYTEXTURE	TextureManager::GetAirShipTexturePtr(int team_type)
{
	MyAssert(team_type >= 0 && team_type < 4, "存在しないチーム番号　%d ", team_type);

	return m_pTexture_array[(int)AIRSHIP_TEXTURE_SODA + team_type];
}


bool TextureManager::LoadTexture()
{
	m_pTexture_array[NUMBER] = new MYTEXTURE("DATA\\TEXTURE\\Number.png");
	m_pTexture_array[TIMER] = new MYTEXTURE("DATA\\TEXTURE\\UI\\Watch.png");

	m_pTexture_array[KILL_ENEMY] = new MYTEXTURE("DATA\\TEXTURE\\UI\\splashed.png");
	m_pTexture_array[SHARE_UP] = new MYTEXTURE("DATA\\TEXTURE\\UI\\share_up.png");
	m_pTexture_array[POWER_UP] = new MYTEXTURE("DATA\\TEXTURE\\UI\\Status_up_UI.png");
	m_pTexture_array[SHARE_OBJ_ICON] = new MYTEXTURE("DATA\\TEXTURE\\UI\\ad_target.png");
	m_pTexture_array[FRIEND_MSG_ICON] = new MYTEXTURE("DATA\\TEXTURE\\UI\\friend_icon.png");
	m_pTexture_array[WATER_DROP_ANIMATION] = new MYTEXTURE("DATA\\Effect\\Animation\\ScreenWaterDrop.png");

	
	//目
	m_pTexture_array[CHR_EYE_CLOSE] = new MYTEXTURE("DATA\\CHR\\COLA_KUN\\eye\\close_eye.png");
	m_pTexture_array[CHR_EYE_DAMAGE] = new MYTEXTURE("DATA\\CHR\\COLA_KUN\\eye\\batu_eye.png");

	m_pTexture_array[CHR_EYE1] = new MYTEXTURE("DATA\\CHR\\COLA_KUN\\eye\\eye0.png");
	m_pTexture_array[CHR_EYE2] = new MYTEXTURE("DATA\\CHR\\COLA_KUN\\eye\\eye1.png");
	m_pTexture_array[CHR_EYE3] = new MYTEXTURE("DATA\\CHR\\COLA_KUN\\eye\\eye2.png");
	m_pTexture_array[CHR_EYE4] = new MYTEXTURE("DATA\\CHR\\COLA_KUN\\eye\\eye3.png");
	m_pTexture_array[CHR_EYE5] = new MYTEXTURE("DATA\\CHR\\COLA_KUN\\eye\\eye4.png");
	m_pTexture_array[CHR_EYE6] = new MYTEXTURE("DATA\\CHR\\COLA_KUN\\eye\\eye5.png");


	m_pTexture_array[COLA_GAUGE] = new MYTEXTURE("DATA\\TEXTURE\\UI\\pet.png");
	m_pTexture_array[COLA_GAUGE_SOUR] = new MYTEXTURE("DATA\\TEXTURE\\UI\\ikura.png");
	m_pTexture_array[COLA_GAUGE_CLIP] = new MYTEXTURE("DATA\\TEXTURE\\UI\\pet_clip.png");

	m_pTexture_array[CHR_TEXTURE_COLA] = new MYTEXTURE("DATA\\CHR\\CHR_TEXTURE\\cola.png");
	m_pTexture_array[CHR_TEXTURE_SODA] = new MYTEXTURE("DATA\\CHR\\CHR_TEXTURE\\soda.png");
	m_pTexture_array[CHR_TEXTURE_MELON] = new MYTEXTURE("DATA\\CHR\\CHR_TEXTURE\\melon.png");
	m_pTexture_array[CHR_TEXTURE_LEMON] = new MYTEXTURE("DATA\\CHR\\CHR_TEXTURE\\lemon.png");

	m_pTexture_array[AD_TEXTURE_COLA] = new MYTEXTURE("DATA\\TEXTURE\\MakerPoster\\red.png");
	m_pTexture_array[AD_TEXTURE_SODA] = new MYTEXTURE("DATA\\TEXTURE\\MakerPoster\\blue.png");
	m_pTexture_array[AD_TEXTURE_MELON] = new MYTEXTURE("DATA\\TEXTURE\\MakerPoster\\green.png");
	m_pTexture_array[AD_TEXTURE_LEMON] = new MYTEXTURE("DATA\\TEXTURE\\MakerPoster\\yellow.png");

	m_pTexture_array[LOGO_TEXTURE_COLA] = new MYTEXTURE("DATA\\TEXTURE\\Logo\\red.png");
	m_pTexture_array[LOGO_TEXTURE_SODA] = new MYTEXTURE("DATA\\TEXTURE\\Logo\\blue.png");
	m_pTexture_array[LOGO_TEXTURE_MELON] = new MYTEXTURE("DATA\\TEXTURE\\Logo\\green.png");
	m_pTexture_array[LOGO_TEXTURE_LEMON] = new MYTEXTURE("DATA\\TEXTURE\\Logo\\yellow.png");

	m_pTexture_array[VENDING_TEXTURE_COLA] = new MYTEXTURE("DATA\\TEXTURE\\Vending\\jihanki_barning.png");
	m_pTexture_array[VENDING_TEXTURE_SODA] = new MYTEXTURE("DATA\\TEXTURE\\Vending\\jihanki_genki.png");
	m_pTexture_array[VENDING_TEXTURE_MELON] = new MYTEXTURE("DATA\\TEXTURE\\Vending\\jihanki_meron.png");
	m_pTexture_array[VENDING_TEXTURE_LEMON] = new MYTEXTURE("DATA\\TEXTURE\\Vending\\jihanki_lemon.png");

	m_pTexture_array[TRUCK_TEXTURE_USUAL] = new MYTEXTURE("DATA\\TEXTURE\\Truck\\truckUsual.png");
	m_pTexture_array[TRUCK_TEXTURE_COLA] = new MYTEXTURE("DATA\\TEXTURE\\Truck\\truckRed.png");
	m_pTexture_array[TRUCK_TEXTURE_SODA] = new MYTEXTURE("DATA\\TEXTURE\\Truck\\truckBlue.png");
	m_pTexture_array[TRUCK_TEXTURE_MELON] = new MYTEXTURE("DATA\\TEXTURE\\Truck\\truckGreen.png");
	m_pTexture_array[TRUCK_TEXTURE_LEMON] = new MYTEXTURE("DATA\\TEXTURE\\Truck\\truckYellow.png");

	m_pTexture_array[AIRSHIP_TEXTURE_COLA] = new MYTEXTURE("DATA\\TEXTURE\\AirShip\\balloon_cola.png");
	m_pTexture_array[AIRSHIP_TEXTURE_SODA] = new MYTEXTURE("DATA\\TEXTURE\\AirShip\\balloon_genki.png");
	m_pTexture_array[AIRSHIP_TEXTURE_MELON] = new MYTEXTURE("DATA\\TEXTURE\\AirShip\\balloon_melon.png");
	m_pTexture_array[AIRSHIP_TEXTURE_LEMON] = new MYTEXTURE("DATA\\TEXTURE\\AirShip\\balloon_lemon.png");

	m_pTexture_array[PET_TEXTURE_COLA] = new MYTEXTURE("DATA\\TEXTURE\\pet\\cola.png");
	m_pTexture_array[PET_TEXTURE_SODA] = new MYTEXTURE("DATA\\TEXTURE\\pet\\genki.png");
	m_pTexture_array[PET_TEXTURE_MELON] = new MYTEXTURE("DATA\\TEXTURE\\pet\\melon.png");
	m_pTexture_array[PET_TEXTURE_LEMON] = new MYTEXTURE("DATA\\TEXTURE\\pet\\lemon.png");


	return true;
}

TextureManager::TEXTURE_ID TextureManager::TextureRegister(LPMYTEXTURE pTex)
{
	for (int i = 0; i < m_TextureMax; i++)
	{
		if (m_pTexture_array[i])continue;
		m_pTexture_array[i] = pTex;
		return (TEXTURE_ID)i;
	}
	delete pTex;
	MyAssert(false, "tex管理総数が最大値に達しました。");

	return ERR_ID;
}

LPMYTEXTURE TextureManager::GetTexturePtr(TEXTURE_ID id)
{
	MyAssert(m_pTexture_array[id], "登録されていないtex要求されました。ID番号= %d", id);
	return m_pTexture_array[id];
}
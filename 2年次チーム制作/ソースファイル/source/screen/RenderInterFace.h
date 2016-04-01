#ifndef __RENDER_INTERFACE_H__
#define __RENDER_INTERFACE_H__

class Character;
class MapObjectInterface;
struct TeamData;

template<class T>
class Rendering_Interface
{
protected:
	char* m_default_Technique;					//使用するShader
	char* m_shadowmap_Technique;	//シャドウマップに使用するテクニック

public:
	/**
	*@note	shaderを使用しない場合はテクニックをnullptrにする
	*/
	Rendering_Interface(char* default_Technique, char* shadowmap_technique) :
		m_default_Technique(default_Technique), m_shadowmap_Technique(shadowmap_technique){}
	virtual void	Render(T* owner) = 0;
	virtual void	Render_ShadowMap(T* owner) = 0;
};

#pragma region ControllObject

class CharacterRenderParam
{
public:
	ControllObject&	viewOwner;
	TeamData&		OwnerteamData;
	Character&		character;
public:
	CharacterRenderParam(
		ControllObject&	viewOwner,
		TeamData& teamData,
		Character& character
		) :
		viewOwner(viewOwner),
		character(character),
		OwnerteamData(teamData){}
};

class R_InterFace_Character : public Rendering_Interface<CharacterRenderParam>
{
	enum outline_color{ RED, GREEN, BLUE, ALPHA };
private:
	float m_color[4];
	float m_outline_Size;

public:
	R_InterFace_Character();
	void	  Render(CharacterRenderParam* owner);
	void	Render_ShadowMap(CharacterRenderParam* owner);
};

#pragma endregion

#pragma region MapObject

class R_InterFace_MapObject : public Rendering_Interface<MapObjectInterface>
{
public:
	R_InterFace_MapObject(char* use_Defaulttec, char* use_shadowmap) :
		Rendering_Interface(use_Defaulttec, use_shadowmap)
	{}
	void	  Render(MapObjectInterface* owner) = 0;
	void	Render_ShadowMap(MapObjectInterface* owner) = 0;
};

class R_InterFace_MainStage : public R_InterFace_MapObject
{
public:
	R_InterFace_MainStage();
	void		Render(MapObjectInterface* owner);
	void		Render_ShadowMap(MapObjectInterface* owner);
};

class R_InterFace_Truck : public R_InterFace_MapObject
{
public:
	R_InterFace_Truck();
	void		Render(MapObjectInterface* owner);
	void		Render_ShadowMap(MapObjectInterface* owner);
};

class R_InterFace_RespawnObject : public R_InterFace_MapObject
{
public:
	R_InterFace_RespawnObject();
	void		Render(MapObjectInterface* owner);
	void		Render_ShadowMap(MapObjectInterface* owner);
};

class RBase_InterFace_ShareObject : public R_InterFace_MapObject
{
public:
	RBase_InterFace_ShareObject(char* use_default_tec, char* use_shadowmap) :
		R_InterFace_MapObject(use_default_tec, use_shadowmap){}
	void		Render(MapObjectInterface* owner) = 0;
	void		Render_ShadowMap(MapObjectInterface* owner) = 0;
};

class R_InterFace_Poster : public RBase_InterFace_ShareObject
{
public:
	R_InterFace_Poster();
	void		Render(MapObjectInterface* owner);
	void		Render_ShadowMap(MapObjectInterface* owner);
};

#pragma endregion

#endif

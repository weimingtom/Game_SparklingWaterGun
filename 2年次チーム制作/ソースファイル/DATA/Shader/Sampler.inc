texture Texture;
sampler DecaleSamp = sampler_state
{
    Texture = <Texture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture LightMap;
sampler LightSamp = sampler_state
{
	Texture = <LightMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

texture	Near_ShadowM;
sampler NShadowSamp = sampler_state
{
	Texture = <Near_ShadowM>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = BORDER;
	AddressV = BORDER;

};


texture	Far_ShadowM;
sampler FShadowSamp = sampler_state
{
	Texture = <Far_ShadowM>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = BORDER;
	AddressV = BORDER;
};

texture	Depth;
sampler DepthSamp = sampler_state
{
	Texture = <Depth>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = BORDER;
	AddressV = BORDER;
};

texture	Toon;
sampler ToonSamp = sampler_state
{
	Texture = <Toon>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture	Reflect;
sampler ReflectSamp = sampler_state
{
	Texture = <Reflect>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = BORDER;
	AddressV = BORDER;
};


texture	Near_ShadowM_Stage;
sampler NShadowSamp_Stage = sampler_state
{
	Texture = <Near_ShadowM_Stage>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = BORDER;
	AddressV = BORDER;

};


texture	Far_ShadowM_Stage;
sampler FShadowSamp_Stage = sampler_state
{
	Texture = <Far_ShadowM_Stage>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = BORDER;
	AddressV = BORDER;
};

texture	ScreenBuffer;
sampler ScreenSamp = sampler_state
{
	Texture = <ScreenBuffer>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = Wrap;
	AddressV = Wrap;
};

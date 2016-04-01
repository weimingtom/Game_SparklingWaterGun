struct VS_OUTPUT
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
};

struct VS_INPUT
{
    float4 Pos    : POSITION;
    float3 Normal : NORMAL;
    float2 Tex	  : TEXCOORD0;
	float4 Color  : COLOR0;
};

struct VS_INPUT_VERTEX
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
};

struct VS_OUTPUT_VERTEX
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
	float4 ProjectionPos : TEXCOORD1;
};

struct VS_OUTPUT_TOON
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float	ViewDepth : TEXCOORD2;
	float4 World_Pos : TEXCOORD3;
	float4 ProjectionPos : TEXCOORD4;
};



struct PS_OUTPUT
{
	float4	Screen		: COLOR0;
	float4	Depth		: COLOR1;
};

struct PS_OUTPUT_SHADOWMAP
{
	float4	Default		: COLOR0;
	float4	MainStage: COLOR1;
};

struct VS_OUTPUT_COLAPOOL
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
	float3 ViewNormal : TEXCOORD1;
	float4 Projection_Pos : TEXCOORD2;
};

struct VS_OUTPUT_TESTPOOL
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
	float ViewDepth : TEXCOORD1;
	float4 Projection_Pos : TEXCOORD2;
};

struct VS_OUTPUT_COLABULLET
{
	float4	Pos		:	POSITION;
	float4	Color	:	COLOR0;
	float2	Tex		:	TEXCOORD0;
	float4	Projection_Pos	:	TEXCOORD1;
};
//**************************************************************************************************
//																									
//		２Ｄ描画用シェーダー		
//
//**************************************************************************************************

//------------------------------------------------------
//		テクスチャサンプラー	
//------------------------------------------------------
texture Texture;
sampler DecaleSamp = sampler_state
{
    Texture = <Texture>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};


texture Blend_Tex;
sampler BlendSamp = sampler_state
{
	Texture = <Blend_Tex>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;

	BorderColor = 0xFFFFFFFF;
	AddressU = BORDER;
	AddressV = BORDER;
};

texture Screen;
sampler ScreenSamp = sampler_state
{
	Texture = <Screen>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

//------------------------------------------------------
//		頂点フォーマット
//------------------------------------------------------
struct VS_2D
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
};

//**************************************************************************************************
//		２Ｄ基本描画シェーダー		
//**************************************************************************************************
//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Basic( VS_2D In) : COLOR
{   

	float4	OUT;

	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, In.Tex );

	return OUT;
}

//------------------------------------------------------
//		合成なし	
//------------------------------------------------------
technique copy
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = None;
		ZEnable          = false;

		PixelShader  = compile ps_2_0 PS_Basic();
    }
}

//------------------------------------------------------
//		加算合成
//------------------------------------------------------
technique add
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = One;
		CullMode         = None;
		ZEnable          = false;

		PixelShader  = compile ps_2_0 PS_Basic();
    }
}
//------------------------------------------------------
//		減算合成
//------------------------------------------------------
technique sub
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = RevSubtract;
		SrcBlend         = SrcAlpha;
		DestBlend        = One;
		CullMode         = None;
		ZEnable          = false;

		PixelShader  = compile ps_2_0 PS_Basic();
    }
}
#include	"DATA\\Shader\\Blur.inc"


float4 PS_Reflect(VS_2D In) : COLOR
{
	float4	OUT;
	float2 tex = In.Tex;
	tex.y = (1.0f - tex.y);
	//	ピクセル色決定
	OUT = In.Color * tex2D(DecaleSamp, tex);

	
	return OUT;
}

technique reflect
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_2_0 PS_Reflect();
	}
}

#include	"DATA\\Shader\\Radiation_Blur.inc"

float4 PS_Blend_Decale(VS_2D In) : COLOR
{
	float4	Out;

	//	ピクセル色決定
	float4 decal = tex2D(DecaleSamp, In.Tex);
	float4 brend_tex = tex2D(BlendSamp, In.Tex);
	Out.rgb = (decal.rgb * (1.0f - blend_rate)) + (brend_tex.rgb * blend_rate);
	Out.a = 1.0;
	return Out;
}

technique blend
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_2_0 PS_Blend_Decale();
	}
}


float4 PS_Position_Effect(VS_2D In) : COLOR
{
	float4	Out;
	//	ピクセル色決定
	float4 texture_color = tex2D(DecaleSamp, In.Tex);
	if (texture_color.a <= 0.1f)
		discard;

	float4 screen_color = tex2D(ScreenSamp, In.Tex);
		return screen_color;
	Out.rgb = In.Color * 0.3 + screen_color * 0.7;
	Out.a = In.Color.a;
	//Out.rgb = (decal.rgb * (1.0f - blend_rate)) + (brend_tex.rgb * blend_rate);
	//Out.a = 1.0;
	return Out;
}


technique position_effect
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_3_0 PS_Position_Effect();
	}
}

#include	"DATA\\Shader\\ColaGauge.inc"
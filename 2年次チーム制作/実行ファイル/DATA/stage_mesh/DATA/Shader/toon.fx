//********************************************************************
//																									
//		トゥーンシェーダー		
//
//********************************************************************
#include "DATA/Shader/shadow.fx"

float OutLineSize = 0.18f; // 輪郭線の太さ
float OutLineSize2 = 0.2f;

float OutLinePower = 5.5;

//------------------------------------------------------
//		頂点フォーマット
//------------------------------------------------------
struct VS_OUTPUT_TOON
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;

	float Level : TEXCOORD1;
	float3 vShadow : TEXCOORD2;
};

//********************************************************************
//
//		輪郭線シェーダー		
//
//********************************************************************

//------------------------------------------------------
//		頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT VS_OutLine( VS_INPUT In )
{
	VS_OUTPUT Out = ( VS_OUTPUT )0;

	// 法線方向に拡大
	float ret = mul( In.Pos, Projection ).z / OutLinePower;
	In.Normal = normalize( In.Normal );
	In.Pos.xyz += In.Normal * ( OutLineSize * ret );
	
	Out.Pos = mul( In.Pos, Projection );
	Out.Tex = In.Tex;
	Out.Color.rgb = 0.2;
	Out.Color.a = 1.0f;

	return Out;
}

VS_OUTPUT VS_OutLine2( VS_INPUT In )
{
	VS_OUTPUT Out = ( VS_OUTPUT )0;

	// 法線方向に拡大
	float ret = mul( In.Pos, Projection ).z / OutLinePower;
	In.Normal = normalize( In.Normal );
	In.Pos.xyz += In.Normal * ( ( OutLineSize + OutLineSize2 ) * ret );

	Out.Pos = mul( In.Pos, Projection );
	Out.Tex = In.Tex;
	Out.Color.rgb = 1;
	Out.Color.a = 0.4;

	return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_OutLine( VS_OUTPUT In ) : COLOR
{
	float4 OUT;

	OUT = In.Color;

	return OUT;
}

//********************************************************************
//
//		トゥーンシェーダー		
//
//********************************************************************
float ToonLevel1 = 0.3f;
float ToonLevel2 = 0.7f;
float ToonShadow1 = 0.65f;
float ToonShadow2 = 0.85f;

bool shadowFlg = false;

//------------------------------------------------------
//		頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT_TOON VS_Toon( VS_INPUT In )
{
	VS_OUTPUT_TOON Out = ( VS_OUTPUT_TOON )0;

	Out.Pos = mul( In.Pos, Projection );
	Out.Tex = In.Tex;
	Out.Color = 1;

	// 法線変換
	float3x3 mat = TransMatrix;
	float3 N = mul( In.Normal, mat );
	N = normalize( N );
	// ライト計算
	float3 vec = normalize( LightDir );
	Out.Level = dot( vec, -N );

	float4 P = mul( In.Pos, TransMatrix );

	if( shadowFlg )
	{
		Out.vShadow = GetShadowTex( P );
	}

	return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Toon( VS_OUTPUT_TOON In ) : COLOR
{
	float4 OUT;

	OUT = tex2D( DecaleSamp, In.Tex );

	if( In.Level < ToonLevel1 )
	{
		OUT.rgb *= ToonShadow1;
	}
	else if( ToonLevel1 <= In.Level && In.Level < ToonLevel2 )
	{
		OUT.rgb *= ToonShadow2;
	}

	if( shadowFlg )
	{
		OUT.rgb *= GetShadow( In.vShadow );

	}

	return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
//------------------------------------------------------
technique toon
{
	// 輪郭線
	pass OutLine
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CW;
		ZEnable = true;

		VertexShader = compile vs_2_0 VS_OutLine( );
		PixelShader = compile ps_2_0 PS_OutLine( );
	}

	// 輪郭線
	pass OutLine2
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CW;
		ZEnable = true;

		VertexShader = compile vs_2_0 VS_OutLine2( );
		PixelShader = compile ps_2_0 PS_OutLine( );
	}

	// トゥーン
	pass Toon
	{
		CullMode = CCW;
		ZWriteEnable = true;

		VertexShader = compile vs_2_0 VS_Toon( );
		PixelShader = compile ps_3_0 PS_Toon( );
	}
}




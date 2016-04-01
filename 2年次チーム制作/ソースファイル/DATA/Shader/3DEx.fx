//********************************************************************
//																									
//		３Ｄ用シェーダー		
//
//********************************************************************
//------------------------------------------------------
//		環境関連
//------------------------------------------------------

#include	"DATA\\Shader\\Variable.inc"
#include	"DATA\\Shader\\Sampler.inc"
#include	"DATA\\Shader\\InputFormat.inc"
#include	"DATA\\Shader\\Function.inc"

//------------------------------------------------------
//		DefaultShader
//------------------------------------------------------
VS_OUTPUT VS_Outline(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	//法線方向に拡大
	In.Normal = normalize(In.Normal);
	In.Pos.xyz += In.Normal * OutlineSize;

	Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color.r = outlinecolor_R;
	Out.Color.g = outlinecolor_G;
	Out.Color.b = outlinecolor_B;
	Out.Color.a = outlinecolor_A;
	return Out;
}

float4 PS_Outline(VS_OUTPUT In) : COLOR0
{
	return In.Color;
}

VS_OUTPUT VS_See_Through(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color.r = outlinecolor_R;
	Out.Color.g = outlinecolor_G;
	Out.Color.b = outlinecolor_B;
	Out.Color.a = outlinecolor_A;
	return Out;
}

float4 PS_See_Through(VS_OUTPUT In) : COLOR0
{
	return In.Color;
}

VS_OUTPUT_TOON VS_Toon(VS_INPUT In)
{
	VS_OUTPUT_TOON Out = (VS_OUTPUT_TOON)0;
	Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	float3x3 mat = (float3x3)TransMatrix;
	float3 N = mul(In.Normal, mat);
	Out.WorldNormal = N;
	Out.Color = In.Color;

	//ワールド空間までの座標
	float4 wpos;
	wpos = mul(In.Pos, TransMatrix);
	Out.World_Pos = wpos;
	float4 temp = Out.World_Pos;
	temp = mul(temp, matView);
	Out.ViewDepth = Out.Pos.z;
	temp = mul(temp, matProjection);
	Out.ProjectionPos = Out.Pos;
	return Out;
}

float specular_level1 = 0.4f;
float specular_level2 = 0.2f;
PS_OUTPUT PS_Toon(VS_OUTPUT_TOON In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;
	//ディフューズ色
	Out.Screen = tex2D(DecaleSamp, In.Tex);
	//Toon計算
	float2 tex;
	float3 normal = In.WorldNormal;
	normal = normalize(normal);
	float3 light = normalize(-LightDir);
	tex.x = max(.0f, dot(normal, light)) * 0.5 + .5f;
	float3 eye = normalize(In.World_Pos.xyz);
	tex.y = .0f;
	float toon_Level = tex2D(ToonSamp,tex).r;
	Out.Screen.rgb *= toon_Level;
	float4 shadow;
	//それぞれのシャドウマップ判定
	//Out.Depth = In.ProjectionPos.z / In.ProjectionPos.w;
	Out.Depth = In.ViewDepth / Zfar;
	Out.Depth.a = 1.0f;
	float depth = In.ProjectionPos.z;
	float shadow_rate = 1.0f;
	if (depth < 7.5f)
	{
		shadow = mulScreen(In.World_Pos, n_ShadowProjection);
		shadow_rate = GetCSM(shadow.xyz, NShadowSamp);
	}
	else
	{
		shadow = mulScreen(In.World_Pos, f_ShadowProjection);
		shadow_rate = GetCSM(shadow.xyz, FShadowSamp);
	}
	//影の場合はスペキュラ計算を飛ばす
	if (shadow_rate < 1.0f && toon_Level >= 0.9f)
	{
		Out.Screen.rgb *= shadow_rate;
		return Out;
	}
	//スペキュラ計算
	float3 view_vec = In.World_Pos.xyz - Camera_Pos;
		view_vec = normalize(view_vec);
	float3 H = -view_vec + light;
		H = normalize(H);
	float rate = dot(normal, H) * .5f + .5f;
	rate = pow(rate, 50);
	if (rate >= 0.5f)
		Out.Screen.rgb += specular_level1;
	else if (rate >= 0.2f)
		Out.Screen.rgb += specular_level2;

	return Out;
}

PS_OUTPUT PS_MainStage_LightMap(VS_OUTPUT_TOON In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;
	//tes
	//Out.Depth = In.ProjectionPos.z / In.ProjectionPos.w;
	Out.Depth = In.ViewDepth / Zfar;
	Out.Depth.a = 1.0f;
	//ピクセル色決定
	Out.Screen = tex2D(DecaleSamp, In.Tex) * In.Color;

	//拡散反射
	float lambert = dot(normalize(-LightDir), normalize(In.WorldNormal));
	lambert =  max((lambert * .5 + .8), .3);



	//ライトマップ取得
	float3 lightmap_color = tex2D(LightSamp, In.Tex).rgb;
	if (lightmap_color.r < .8f)
	{
		Out.Screen.rgb *= ShadowLevel;
		return Out;
	}

	float4 shadow;
	float depth = In.ProjectionPos.z;
	//float near_ShadowLevel;
	//float far_ShadowLevel;
	//shadow = mulScreen(In.World_Pos, n_ShadowProjection);
	//near_ShadowLevel = max(ShadowLevel, GetCSM(shadow.xyz, NShadowSamp_Stage));
	//shadow = mulScreen(In.World_Pos, f_ShadowProjection);
	//far_ShadowLevel = max(ShadowLevel, GetCSM(shadow.xyz, FShadowSamp_Stage));
	////float s = lerp(near_ShadowLevel, far_ShadowLevel, min(1.0f, (depth / 7.0)));
	//float s = lerp(near_ShadowLevel, far_ShadowLevel, sigmoid(5.0,(depth / 7.0) + 0.7));
	//Out.Screen.rgb *= s;

	if (depth < 5.0f)
	{
		shadow = mulScreen(In.World_Pos, n_ShadowProjection);
		Out.Screen.rgb *= max(ShadowLevel, GetCSM(shadow.xyz, NShadowSamp_Stage));
	}
	else
	{
		shadow = mulScreen(In.World_Pos, f_ShadowProjection);
		Out.Screen.rgb *= max(ShadowLevel, GetCSM(shadow.xyz, FShadowSamp_Stage));
	}

	return Out;
}

PS_OUTPUT PS_MainStage_RealTimeShadow(VS_OUTPUT_TOON In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;
	//tes
	//Out.Depth = In.ProjectionPos.z / In.ProjectionPos.w;
	Out.Depth = In.ViewDepth / Zfar;
	Out.Depth.a = 1.0f;
	//ピクセル色決定
	Out.Screen = tex2D(DecaleSamp, In.Tex) * In.Color;


	//拡散反射
	float lambert = dot(normalize(-LightDir), normalize(In.WorldNormal));
	//lambert = min((lambert * .5 + 1.0),.8);
	lambert = lambert*0.4f + 0.6f;
	Out.Screen.rgb *= lambert;

	float4 shadow;
	float depth = In.ProjectionPos.z;
	float near_ShadowLevel;
	float far_ShadowLevel;
	shadow = mulScreen(In.World_Pos, n_ShadowProjection);
	near_ShadowLevel = max(ShadowLevel, GetCSM(shadow.xyz, NShadowSamp_Stage));
	shadow = mulScreen(In.World_Pos, f_ShadowProjection);
	far_ShadowLevel = max(ShadowLevel, GetCSM(shadow.xyz, FShadowSamp_Stage));
	float s = lerp(near_ShadowLevel, far_ShadowLevel, sigmoid(30.0,(depth / 12.0),0.5));
	Out.Screen.rgb *= s;

	//if (depth < 5.0f)
	//{
	//	shadow = mulScreen(In.World_Pos, n_ShadowProjection);
	//	Out.Screen.rgb *= max(ShadowLevel, GetCVSM(shadow.xyz, NShadowSamp_Stage));
	//}
	//else
	//{
	//	shadow = mulScreen(In.World_Pos, f_ShadowProjection);
	//	Out.Screen.rgb *= max(ShadowLevel, GetCVSM(shadow.xyz, FShadowSamp_Stage));
	//}

	return Out;
}



PS_OUTPUT PS_Truck(VS_OUTPUT_TOON In)
{
	PS_OUTPUT Out = (PS_OUTPUT)0;
	//Out.Depth = In.ProjectionPos.z / In.ProjectionPos.w;
	Out.Depth = In.ViewDepth / Zfar;
	Out.Depth.a = 1.0;
	float4 tex_col = tex2D(DecaleSamp, In.Tex);
	if (tex_col.a < 0.2)
		discard;
	Out.Screen = tex_col * In.Color;
	
	return Out;
}



#include "DATA\\Shader\\ShadowMap.inc"

VS_OUTPUT_COLAPOOL VS_ColaPool(VS_INPUT In)
{
	VS_OUTPUT_COLAPOOL Out = (VS_OUTPUT_COLAPOOL)0;
	Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color = In.Color;

	//法線はビュー空間まで変換
	float3x3 mat = (float3x3)TransMatrix;
	mat = mul(mat, (float3x3)matView);
	Out.ViewNormal = mul(In.Normal, mat);
	float4 pos = Out.Pos;
	pos.y *= -1.0f;
	Out.Projection_Pos = pos;
	return Out;
}


float4 PS_ColaPool(VS_OUTPUT_COLAPOOL In) : COLOR0
{
	float4 Out;
	//return 1;
	float4 tex_pos = In.Projection_Pos;
	tex_pos.xyz = tex_pos.xyz / tex_pos.w;
	tex_pos.xy = tex_pos.xy * 0.5f + 0.5f;
	float alpha = tex2D(DecaleSamp, In.Tex).a;
	float rate = 0.80f;
	Out = tex2D(ReflectSamp, tex_pos.xy)*In.Color;
	Out.a = alpha;
	Out.rgb = Out.rgb * (1.0 - rate) + colacolor * rate;
	return Out;
}

VS_OUTPUT_TESTPOOL VS_TestPool(VS_INPUT In)
{
	VS_OUTPUT_TESTPOOL Out = (VS_OUTPUT_TESTPOOL)0;
	Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color = In.Color;

	float4 P = In.Pos;
	P = mul(P, TransMatrix);
	P = mul(P, matView);
	Out.ViewDepth = P.z;
	float4 pos = Out.Pos;
	Out.Projection_Pos = pos;
	return Out;
}


float4 PS_TestPool(VS_OUTPUT_TESTPOOL In) : COLOR0
{
	float4 Out;
	float4 tex_pos = In.Projection_Pos;
	tex_pos.xy = tex_pos.xy / tex_pos.w;
	tex_pos.xy = tex_pos.xy * 0.5f + 0.5f;
	tex_pos.y = 1.0f -tex_pos.y;
	tex_pos.z /= Zfar;
	float4 texcolor = tex2D(DecaleSamp, In.Tex);

	Out = texcolor*In.Color;

	float depth = tex2D(DepthSamp, tex_pos.xy).r;
	float dist = abs(depth -tex_pos.z);

	if (depth > tex_pos.z && dist >= 0.0025f)
		discard;

	if (Out.a <= 0.05f)
		discard;
	
	return Out;
}

//VS_OUTPUT_COLABULLET VS_Bullet(VS_INPUT In)
//{
//	VS_OUTPUT_COLABULLET Out = (VS_OUTPUT_COLABULLET)0;
//	Out.Pos = mul(In.Pos, Projection);
//	Out.Tex = In.Tex;
//	Out.Color = In.Color;
//	Out.Projection_Pos = Out.Pos;
//	Out.Projection_Pos.y *= -1.0f;
//	return Out;
//}
//
//
//float4 PS_Bullet(VS_OUTPUT_TESTPOOL In) : COLOR0
//{
//	float4 Out;
//	float4 tex_pos = In.Projection_Pos;
//	tex_pos.xy = tex_pos.xy / tex_pos.w;
//	tex_pos.xy = tex_pos.xy * 0.5f + 0.5f;
//	float4 texcolor = tex2D(DecaleSamp, In.Tex);
//	if (texcolor.a == .0f)
//		discard;
//
//	//スクリーンカラー取得
//	float4 screen_color = tex2D(ScreenSamp, tex_pos.xy);
//	float rate = .6f;
//	Out.rgb = In.Color * texcolor.rgb;
//	//Out.rgb *= screen_color.rgb * rate + Out.rgb * (1.0f - rate);
//	Out.rgb = screen_color.rgb;
//	Out.a = 1.0f;
//	return screen_color;
//}


VS_OUTPUT_VERTEX VS_ScreenPosEffect(VS_INPUT_VERTEX In)
{
	VS_OUTPUT_VERTEX Out = (VS_OUTPUT_VERTEX)0;
	float4 screen_pos = In.Pos;
	screen_pos.x = screen_pos.x * 2.0 - 1.0;
	screen_pos.y = screen_pos.y * 2.0 - 1.0;
	screen_pos.y *= -1.0;
	Out.Pos = screen_pos;
	Out.ProjectionPos = screen_pos;
	Out.Color = In.Color;
	Out.Tex = In.Tex;
	return Out;
}

float4 PS_ScreenPosEffect(VS_OUTPUT_VERTEX In) : COLOR0
{
	float4 Out = In.Color;
	float4 texture_color = tex2D(DecaleSamp, In.Tex);
	if (texture_color.a <= 0.1)
		discard;
	float2 test;
	test.x = .0;
	test.y = 0.02;
	//スクリーン空間の座標取得
	float4 screen_pos = In.ProjectionPos;
	screen_pos.xy = screen_pos.xy *0.5 + 0.5;
	screen_pos.y *= -1.0;
	screen_pos.xy /= screen_pos.w;
	float4 screen_color = tex2D(ScreenSamp, screen_pos + test );
	Out.rgb = screen_color.rgb * 0.3+ Out.rgb *0.7;
	return Out;
}

technique screenposition_effect
{
	pass Toon
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = false;

		VertexShader = compile vs_3_0 VS_ScreenPosEffect();
		PixelShader = compile ps_3_0 PS_ScreenPosEffect();
	}
}


#include	"DATA\\Shader\\technic.inc"

//------------------------------------------------------
//		環境関連
//------------------------------------------------------
float4x4 Projection;	//	投影変換行列
float4x4 TransMatrix;	//	ワールド変換行列
//------------------------------------------------------
//		環境光
//------------------------------------------------------
float3	AmbientColor = { 0.2f, 0.2f, 0.2f };
//------------------------------------------------------
//		UVアニメーション
//------------------------------------------------------
float animeU;
float animeV;
//------------------------------------------------------
//		テクスチャサンプラー	
//------------------------------------------------------
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

//////
texture NormalMap;
sampler NormalSamp = sampler_state
{
    Texture = <NormalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

texture HeightMap;
sampler HeightSamp = sampler_state
{
    Texture = <HeightMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

//------------------------------------------------------
//		環境用テクスチャサンプラー	
//------------------------------------------------------
texture  EnvTex;
sampler  EnvironmentSamp = sampler_state
{
    Texture    = <EnvTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

float	EnvParam = 0.5f;	//	映りこみの割合




//------------------------------------------------------
//		頂点フォーマット
//------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;

	float3 vLight	: TEXCOORD1;//	ライトベクトル（頂点空間）
	float3 vE	: TEXCOORD2;//	視線ベクトル（頂点空間）

	float3 plight: TEXCOORD3;//点光源用
};
float3 plight_color = { 1,0,0.5f };
float3 plight_pos = { 0,0.5f,0 };
float plight_range = 5.0f;

struct VS_INPUT
{
    float3 Normal	: NORMAL;
    float4 Pos		: POSITION;
    float2 Tex		: TEXCOORD0;
	float4 Color : COLOR0;
};

struct VS_OUTPUT_FOG
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;

	float  Fog		: TEXCOORD1;

	float4 worldPos    : TEXCOORD2;		//	ワールドの座標
    float3 worldNormal : TEXCOORD3;		//	ワールドの法線

};

//	トゥーン用頂点
struct VS_OUTPUT_TOON
{
    float4 Pos		: POSITION;
    float4 Color		: COLOR0;
    float2 Tex		: TEXCOORD0;
    
	float  Level		: TEXCOORD1;
};

float	OutlineSize = 1.0f;	//	輪郭線の太さ

//　擬似HDR表現
struct VS_HDR
{
    float4 Pos		: POSITION;
    float3 wPos		: TEXCOORD2;
    float3 Normal	: TEXCOORD1;
    float2 Tex		: TEXCOORD0;

};

//------------------------------------------------------
//		平行光
//------------------------------------------------------
float3	DirLightVec = { -1, 0, 0 };
float3	DirLightColor = { 0.5, 0.5,0.5 };

float3 DirLight( float3 normal )
{
	float3 vec = normalize(DirLightVec);
	float intensity = max( 0, dot( vec, -normal ) );

	return DirLightColor * intensity;
}

float3 DirLight2(float3 light,  float3 normal )
{
	float3 vec = normalize(light);
	float intensity = max( 0, dot( vec, -normal ) );

	return DirLightColor * intensity;
}
//------------------------------------------------------
//		半球ライティング
//------------------------------------------------------
float3	SkyColor = { 0.3f, 0.4f, 0.9f };
float3	GroundColor = { 0.9f, 0.3f, 0.1f };

float3 HemiSphereLight( float3 normal )
{
	float	rate = normal.y*0.5f + 0.5f;
	float3	color = GroundColor*(1-rate) + SkyColor * rate;
	
	return color;
}
//------------------------------------------------------
//		フォグ
//------------------------------------------------------
float  FogNear = 10.0f;
float  FogFar  = 25.0f;
float3 FogColor = { 0.9f, 0.95f, 1 };

float Fog( float3 pos )
{
	float	rate;
	rate = (FogFar-pos.z) / (FogFar-FogNear);
	rate = saturate(rate);

	return rate;
}


//------------------------------------------------------
//		スペキュラ
//------------------------------------------------------

float3 ViewPos = { 0, 0, 0 };

float Specular( float3 pos, float3 N )
{
	float	rate;
	
	float3 vE = normalize( pos - ViewPos );	//	視線ベクトル
	float3 vL = normalize( DirLightVec );	//	ライトベクトル
	float3 H = -normalize( vE + vL );		//	視線とライトのハーフ
	
	rate = max( 0, dot( H, N ) );		//	明るさ算出
	rate = pow( rate, 30 );				//	調整（乗）

	return rate;
}
float HDRSpecular( float3 pos, float3 normal )
{
	float	sp;

	float3 H = normalize( ViewPos - pos );
	H = normalize( H - DirLightVec );

	sp = dot(normal, H);
	sp = max(0, sp);
	sp = pow(sp, 5);

	return sp;
}

texture  SpecularMap;
sampler  SpecularSamp = sampler_state
{
    Texture    = <SpecularMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};
//------------------------------------------------------
//		頂点シェーダー	
//------------------------------------------------------
float Alpha;
VS_OUTPUT VS_Basic( VS_INPUT In )
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color = 1.0f;
	//	環境光適用
	Out.Color.rgb = AmbientColor;
	//	法線変換	
	Out.Color.a = Alpha;

	float3x3 mat = TransMatrix;
	float3 N = mul( In.Normal, mat );
	N = normalize(N);

	//	平行光適用
	Out.Color.rgb += DirLight(N);

    return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Basic( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	//	ピクセル色決定
	OUT = In.Color* tex2D( DecaleSamp, In.Tex );

	return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
//------------------------------------------------------
technique copy
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZEnable          = true;

		VertexShader = compile vs_2_0 VS_Basic();
		PixelShader  = compile ps_2_0 PS_Basic();
    }
}


//------------------------------------------------------
//		頂点シェーダー
//------------------------------------------------------
VS_OUTPUT_FOG VS_Lighting( VS_INPUT In )
{
    VS_OUTPUT_FOG Out = (VS_OUTPUT_FOG)0;

    Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color.a = 1;

	//	法線変換	
	float3x3 mat = TransMatrix;
	float3 N = mul( In.Normal, mat );
	N = normalize(N);

	//	平行光適用
	Out.Color.rgb += DirLight(N);

	//	半球ライティング適用
	Out.Color.rgb += HemiSphereLight(N);

	//	フォグ
	Out.Fog = Fog(Out.Pos);
		//	グローバルポジション&法線
	Out.worldPos = mul(In.Pos, TransMatrix );
	float3x3   m = (float3x3)TransMatrix;
	Out.worldNormal = mul(In.Normal, m );

    return Out;
}
//------------------------------------------------------
//		頂点シェーダー
//------------------------------------------------------
VS_OUTPUT_FOG VS_Lighting2( VS_INPUT In )
{
    VS_OUTPUT_FOG Out = (VS_OUTPUT_FOG)0;

    Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color.a = 1;

	//	法線変換	
	float3x3 mat = TransMatrix;
	float3 N = mul( In.Normal, mat );
	N = normalize(N);

	//	平行光適用
	Out.Color.rgb += DirLight(N);

	//	半球ライティング適用
	Out.Color.rgb += HemiSphereLight(N);

	//	フォグ
	Out.Fog = Fog(Out.Pos);

	//	グローバルポジション&法線
	Out.worldPos = mul(In.Pos, TransMatrix );
	float3x3   m = (float3x3)TransMatrix;
	Out.worldNormal = mul(In.Normal, m );

    return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Lighting( VS_OUTPUT_FOG In) : COLOR
{   
	float4	OUT;
	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, In.Tex );
			//	フォグ採用
	OUT.rgb = (OUT.rgb * In.Fog) + (FogColor * (1-In.Fog));

	return OUT;
}


float4 PS_Lighting2( VS_OUTPUT_FOG In) : COLOR
{   
	float4	OUT;
	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, In.Tex );
	
	In. worldNormal = normalize(In.worldNormal);
//	スペキュラ
	OUT.rgb += Specular( In.worldPos, In.worldNormal );

	//	フォグ採用
	OUT.rgb = (OUT.rgb * In.Fog) + (FogColor * (1-In.Fog));

	return OUT;
}


//------------------------------------------------------
//		半球ライティング＋フォグ
//------------------------------------------------------
technique lighting
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZEnable          = true;

		VertexShader = compile vs_2_0 VS_Lighting();
		PixelShader  = compile ps_2_0 PS_Lighting();
    }
}
technique lighting2
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZEnable          = true;

		VertexShader = compile vs_2_0 VS_Lighting2();
		PixelShader  = compile ps_2_0 PS_Lighting2();
    }
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Env( VS_OUTPUT_FOG In) : COLOR
{   
	float4	OUT;
	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, In.Tex );

	In.worldNormal = normalize(In.worldNormal);

	//	環境マップＵＶ計算
	float2 euv = (In.worldNormal.xy * 0.5f) + 0.5f;
	//euv.y = 1-euv.y;
	//	環境マップ
	float4 Env = tex2D( EnvironmentSamp, euv );
	//	環境マップブレンド
	OUT = (OUT * (1-EnvParam)) + (Env * EnvParam);

	//	スペキュラ
	OUT.rgb += Specular( In.worldPos, In.worldNormal );

	//	フォグ採用
	OUT.rgb = (OUT.rgb * In.Fog) + (FogColor * (1-In.Fog));

	return OUT;
}

//------------------------------------------------------
//		環境マッピング
//------------------------------------------------------
technique environment
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZEnable          = true;

		VertexShader = compile vs_2_0 VS_Lighting();
		PixelShader  = compile ps_2_0 PS_Env();
    }
}


//------------------------------------------------------
//		アウトライン頂点シェーダー	
//------------------------------------------------------
VS_OUTPUT VS_Outline( VS_INPUT In )
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

	//	法線方向に拡大
	In.Normal = normalize(In.Normal);
	//In.Normal = mul(In.Normal,(float3x3)TransMatrix);
	In.Pos.xyz += In.Normal * OutlineSize;

	//
	Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color.rgb = 0;
	Out.Color.a = 1;

    return Out;
}


float	ToonLevel = 0.3f;	//	影のレベル
float	ToonShadow = 0.4f;	//	影の濃さ


VS_OUTPUT_TOON VS_Toon( VS_INPUT In )
{
    VS_OUTPUT_TOON Out = (VS_OUTPUT_TOON)0;
//	float3x3 mat= mul(TransMatrix, (float3x3)Projection);
    Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Color = 1;

	//	法線変換	
	float3x3 mat = (float3x3)TransMatrix;
	float3 N = mul( In.Normal, mat );
	N = normalize(N);
	//	ライト計算
	float3 vec = normalize(DirLightVec);
	Out.Level = dot( vec, -N );

    return Out;
}


//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Outline( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	//	ピクセル色決定
	OUT = In.Color;
	return OUT;
}

float4 PS_Toon( VS_OUTPUT_TOON In) : COLOR
{   
	float4	OUT;
	//	ピクセル色決定
	OUT = tex2D( DecaleSamp, In.Tex );
	//	レベル未満を影にする
	if( In.Level < ToonLevel ) OUT.rgb *= ToonShadow;

	return OUT;
}


technique toon
{
	//	輪郭線
    pass Outline
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CW;
		ZEnable          = true;

		VertexShader = compile vs_2_0 VS_Outline();
		PixelShader  = compile ps_2_0 PS_Outline();
    }

	//	トゥーン
    pass Toon
    {
		CullMode         = CCW;
		ZWriteEnable     = true;

		VertexShader = compile vs_2_0 VS_Toon();
		PixelShader  = compile ps_2_0 PS_Toon();
    }

}

VS_OUTPUT VS_FullFX( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos   = mul(In.Pos, Projection);
	Out.Color = In.Color;
	Out.Tex   = In.Tex;

	Out.Color.rgb = AmbientColor;
	float4 P = mul(In.Pos, TransMatrix);
	float3x3	mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	N = normalize(N);

	// 半球ライティング
	//Out.Color.rgb = HemiLight(N);

	//	頂点ローカル座標系算出
	float3	vx;
	float3	vy = { 0, 1, 0.001f };
	vx = cross( vy, N );
	vx = normalize( vx );
	vy = cross( vx, N  );
	vy = normalize( vy );

	//　点光源ベクトル補正
	float3 pvec = (P.xyz-plight_pos)/plight_range;
	Out.plight.x = dot(vx, pvec);
	Out.plight.y = dot(vy, pvec);
	Out.plight.z = dot(N, pvec);
	//	ライトベクトル補正
	Out.vLight.x = dot(vx, DirLightVec);
	Out.vLight.y = dot(vy, DirLightVec);
	Out.vLight.z = dot(N, DirLightVec);

	// 視線ベクトル補正
	float3 E = P - ViewPos;   // 視線ベクトル
	Out.vE.x = dot(vx, E);
	Out.vE.y = dot(vy, E);
	Out.vE.z = dot(N, E);

	return Out;
}

float4 PS_Level1( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	float2 Tex = In.Tex;

	//	法線取得
	float3 N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

	//	ライト計算
	In.vLight   = normalize( In.vLight );
	float3 light = DirLight2(In.vLight, N);

	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, Tex );
	OUT.rgb = (OUT.rgb + light );

	return OUT;
}

technique level1
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZEnable          = true;

		VertexShader = compile vs_2_0 VS_FullFX();
		PixelShader  = compile ps_2_0 PS_Level1();
    }
}
float4 PS_Level2( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	float2 Tex = In.Tex;

	//	パララックスディスプレースメント
	float h = tex2D( HeightSamp, Tex ).r-0.5f;
	float3 E = normalize(In.vE);
	Tex.x += 0.04f * h * E.x;
	Tex.y -= 0.04f * h * E.y;

	//	法線取得
	float3 N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

	//	ライト計算
	In.vLight   = normalize( In.vLight );
	float3 light = DirLight2(In.vLight, N);

	//点光源
	float plight_pow = max( 0, ( 1 - length(In.plight) ) );
	float3 pN = normalize(In.plight);
	plight_pow *= max(0, -dot(pN, N));
	light += plight_pow * plight_color;

	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, Tex );
	OUT.rgb = (OUT.rgb + light);

	return OUT;
}

technique level2
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZEnable          = true;

		VertexShader = compile vs_2_0 VS_FullFX();
		PixelShader  = compile ps_2_0 PS_Level2();
    }
}

float4 PS_Level3( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	float2 Tex = In.Tex;

	//	パララックスディスプレースメント
	float h = tex2D( HeightSamp, Tex ).r-0.5f;
	float3 E = normalize(In.vE);
	Tex.x += 0.04f * h * E.x;
	Tex.y -= 0.04f * h * E.y;

	//	法線取得
	float3 N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

	//	ライト計算
	In.vLight   = normalize( In.vLight );
	float3 light = DirLight2(In.vLight, N);

	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, Tex );
	OUT.rgb = (OUT.rgb + light);

	//specular
	float3 R = reflect(-E, N);
	float4 sp = tex2D(SpecularSamp, Tex);
	OUT.rgb += pow(max(0, dot(R, In.vLight)), 10)*sp;

	return OUT;
}
technique level3
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZEnable          = true;
		ZWriteEnable    = true;

		VertexShader = compile vs_2_0 VS_FullFX();
		PixelShader  = compile ps_2_0 PS_Level3();
    }
}
float4 Environment(float3 normal)
{
	float4 color;
	float2 uv;
	uv = normal.xy + 0.5f + 0.5f;
	uv.y = -uv.y;
	color = tex2D(EnvironmentSamp,uv);
	return color;
}
float4 PS_Level4( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	float2 Tex = In.Tex;

	//	パララックスディスプレースメント
	float h = tex2D( HeightSamp, Tex ).r-0.5f;
	float3 E = normalize(In.vE);
	Tex.x += 0.04f * h * E.x;
	Tex.y -= 0.04f * h * E.y;

	//	法線取得
	float3 N = tex2D( NormalSamp, Tex ).xyz * 2.0f - 1.0f;

	//	ライト計算
	In.vLight   = normalize( In.vLight );
	float3 light = DirLight2(In.vLight, N);

	//点光源
	float plight_pow = max( 0, ( 1 - length(In.plight) ) );
	float3 pN = normalize(In.plight);
	plight_pow *= max(0, -dot(pN, N));
	light += plight_pow * plight_color;

	//specular
	float3 R = reflect(-E, N);
	float4 sp = tex2D(SpecularSamp, Tex);
	
	float3 env = Environment(R)*(1-sp.a);


	//	ピクセル色決定
	OUT = In.Color * tex2D( DecaleSamp, Tex );
	OUT.rgb = (OUT.rgb + light + env);

	
	OUT.rgb += pow(max(0, dot(R, In.vLight)), 10)*sp;

	return OUT;
}
technique level4
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZEnable          = true;
		ZWriteEnable    = true;

		VertexShader = compile vs_2_0 VS_FullFX();
		PixelShader  = compile ps_2_0 PS_Level4();
    }
}

//頂点シェーダー
VS_HDR VS_Specular( VS_INPUT In )
{
	VS_HDR Out = (VS_HDR)0;

	Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;

	//スペキュラ計算用座標
	Out.wPos = mul(In.Pos, TransMatrix);
	//スペキュラ計算用法線
	float3x3 mat = TransMatrix;
	float3 N = mul(In.Normal, mat);
	Out.Normal = normalize(N);
	return Out;
}
//ピクセルシェーダー
float4 PS_Specular( VS_HDR In ) : COLOR
{
	float4 OUT;
	//スペキュラ計算
	float s = HDRSpecular( In.wPos, In.Normal );
	//
	float4 sp = tex2D( SpecularSamp, In.Tex ) * s;
	//ピクセル色決定
	OUT.rgb = sp*0.5f;
	OUT.a = 1;

	return OUT;
}

//テクニック
technique specular
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		CullMode         = CCW;
		ZEnable          = true;
		ZWriteEnable    = true;

		VertexShader = compile vs_2_0 VS_Specular();
		PixelShader  = compile ps_2_0 PS_Specular();
    }
}
//------------------------------------------------------
//		UVアニメーション
//------------------------------------------------------
struct VS_EFFECT
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
};
VS_EFFECT VS_Effect( VS_INPUT In )
{
    VS_EFFECT Out = (VS_EFFECT)0;

    Out.Pos = mul(In.Pos, Projection);
	Out.Tex = In.Tex;
	Out.Tex.x += animeU;
	Out.Tex.y += animeV;
	Out.Color = 1.0f;
	//	環境光適用
	Out.Color.rgb = AmbientColor;
	//	法線変換	
	float3x3 mat = TransMatrix;
	float3 N = mul( In.Normal, mat );
	N = normalize(N);

	//	平行光適用
	Out.Color.rgb += DirLight(N);

    return Out;
}

//------------------------------------------------------
//		ピクセルシェーダー	
//------------------------------------------------------
float4 PS_Effect( VS_OUTPUT In) : COLOR
{   
	float4	OUT;
	//	ピクセル色決定
	OUT = In.Color* tex2D( DecaleSamp, In.Tex );

	return OUT;
}

//------------------------------------------------------
//		通常描画テクニック
//------------------------------------------------------
technique UVanime

{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = One;
		CullMode         = None;
		ZWriteEnable          = true;

		VertexShader = compile vs_2_0 VS_Effect();
		PixelShader  = compile ps_2_0 PS_Effect();
    }
}


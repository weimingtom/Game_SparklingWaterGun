
/*
定数
*/

// 変換行列
float4x4 TransMatrix;
float4x4 matProjection;
float4x4 MatView;

//平行光（Lambert）
float3 DirLight = {1.0f,.0f,.0f};

/*
テスクチャサンプラ
*/

//	ここにモデルの部位ごとのテクスチャが送られてくる
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

//	ここにモデルの部位ごとのテクスチャが送られてくる
texture ToonTexture;
sampler ToonSamp = sampler_state
{
	Texture = <ToonTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};



struct VS_OUTPUT
{
	float4 Pos		: POSITION;
	float4 Color	: COLOR0;
	float2 Tex		: TEXCOORD0;
	float3 Normal	: TEXCOORD1;
};


/*
頂点シェーダー (頂点の位置をローカルからスクリーン上に持っていくシェーダー)
*/

VS_OUTPUT BasicVS(
	float4 pos			: POSITION,	//頂点のローカル位置
	float3 normal	: NORMAL,	//頂点のローカルでの法線のベクトル
	float4 Color  : COLOR0,	//頂点の色
	float2 Tex	  : TEXCOORD0//頂点のテクスチャ座標
	){

	// 頂点座標、テクスチャ座標
	VS_OUTPUT Out = (VS_OUTPUT)1;

	pos = mul(pos, TransMatrix);		//TransMatrix = "ワールド変換する"
	pos = mul(pos, MatView);		   //MatView="カメラの視点に持っていく"
	pos = mul(pos, matProjection);	//matProjection="画面上に持っていく"

	Out.Pos = pos;
	Out.Tex = Tex;

	//DirLight関係5
	//DirLight = normalize(DirLight);//正規化

	float3 world_normal = mul(normal, TransMatrix);
	world_normal = normalize(world_normal);

	float v=dot(-DirLight, world_normal);

	Out.Color.rgb = (v + 1)*0.5f;

	return Out;
}

/*5
ピクセルシェーダー
*/
float4 BasicPS(VS_OUTPUT In) : COLOR
{
	//float2 toon_tex_pos;
	//toon_tex_pos.y = 0.5f;
	//toon_tex_pos.x = In.Color.r;

//	float4 toon_val = tex2D ( ToonSamp, toon_tex_pos );

	float4 Out = /*toon_val**/tex2D ( DecaleSamp, In.Tex );
	return Out;
}

/*
テクニック
*/
technique Basic {
	pass P0 {
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;

		VertexShader = compile vs_3_0 BasicVS();
		PixelShader = compile ps_3_0 BasicPS();
	}
}



VS_OUTPUT SetColorVS(
	float4 pos			: POSITION,	//頂点のローカル位置
	float3 normal : NORMAL,	//頂点のローカルでの法線のベクトル
	float4 Color : COLOR0,	//頂点の色
	float2 Tex : TEXCOORD0//頂点のテクスチャ座標
	){

	// 頂点座標、テクスチャ座標
	VS_OUTPUT Out = (VS_OUTPUT) 1;

	pos = mul ( pos, TransMatrix );		//TransMatrix = "ワールド変換する"
	pos = mul ( pos, MatView );		   //MatView="カメラの視点に持っていく"
	pos = mul ( pos, matProjection );	//matProjection="画面上に持っていく"

	Out.Pos = pos;
	Out.Tex = Tex;
	Out.Color.rgb = 1.0f;
	Out.Color.a = 0;

	return Out;
}

/*
ピクセルシェーダー
*/
float4 SetColorPS ( VS_OUTPUT In ) : COLOR
{
	float4 Color;

	Color.gb = 0;
	Color.r = 1;
	Color.a = 1;
	//return 1;

	return Color;
}

technique set_color
{
	pass P0 {
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;
		CullMode = NONE;
		VertexShader = compile vs_3_0 SetColorVS();
		PixelShader = compile ps_3_0 SetColorPS();
	}
}




VS_OUTPUT BodyLineVS (
	float4 pos			: POSITION,	//頂点のローカル位置
	float3 normal : NORMAL,	//頂点のローカルでの法線のベクトル
	float4 Color : COLOR0,	//頂点の色
	float2 Tex : TEXCOORD0//頂点のテクスチャ座標
	)
{

	// 頂点座標、テクスチャ座標
	VS_OUTPUT Out = (VS_OUTPUT) 1;

	pos.xyz += normalize ( normal ) * 1;

	pos = mul ( pos, TransMatrix );		//TransMatrix = "ワールド変換する"
	pos = mul ( pos, MatView );		   //MatView="カメラの視点に持っていく"
	pos = mul ( pos, matProjection );	//matProjection="画面上に持っていく"
	pos.z += 0.001f;

	Out.Pos = pos;
	Out.Tex = Tex;

	//DirLight関係5
	//	DirLight = normalize(DirLight);//正規化

	float3 world_normal = mul ( normal, ( float3x3 )TransMatrix );	//ワールド変換した法線
		world_normal = normalize ( world_normal );

	float v = dot ( -DirLight, world_normal );

	Out.Color.rgb = ( v + 1 )*0.5f;

	return Out;
}

/*
ピクセルシェーダー
*/
float4 BodyLinePS ( VS_OUTPUT In ) : COLOR
{
	float4 Color;

	Color.gb = 0;
	Color.r = 1;
	Color.a = 1;
	//return 1;

	return Color;
}

technique Body_line
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = true;
		CullMode = CW;
		VertexShader = compile vs_3_0 BodyLineVS ( );
		PixelShader = compile ps_3_0 BodyLinePS ( );
	}
}
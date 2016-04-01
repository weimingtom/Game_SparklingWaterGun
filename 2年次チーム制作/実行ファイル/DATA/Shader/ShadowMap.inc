float4x4 ShadowProjection;

struct VS_SHADOW
{
	float4 Pos : POSITION;
	float4 Color : TEXCOORD0;	//頂点色
};


VS_SHADOW VS_ShadowBuf(float4 Pos : POSITION)
{
	VS_SHADOW Out;
	//座標変換

	float4x4 mat = mul(TransMatrix, ShadowProjection);
	Out.Pos = mul(Pos, mat);
	Out.Color = Out.Pos;
	return Out;
}

PS_OUTPUT_SHADOWMAP PS_ShadowBuf(VS_SHADOW In) : COLOR0
{
	PS_OUTPUT_SHADOWMAP Out = (PS_OUTPUT_SHADOWMAP)0;
	Out.Default.r = In.Color.b / In.Color.w;
	Out.Default.g = Out.Default.r  * Out.Default.r ;
	Out.Default.b = .0f;
	Out.Default.a = .0f;

	Out.MainStage.r = In.Color.b / In.Color.w;
	Out.MainStage.g = Out.Default.r  * Out.Default.r ;
	Out.MainStage.b = .0f;
	Out.MainStage.a = .0f;
	return Out;
}

technique ShadowBuf
{
	pass Pass0
	{
		AlphaBlendEnable = false;
		ZWriteEnable = true;
		CullMode = CCW;

		VertexShader = compile vs_3_0 VS_ShadowBuf();
		PixelShader = compile ps_3_0 PS_ShadowBuf();
	}
}




VS_SHADOW VS_ShadowBuf_MainStage(float4 Pos : POSITION)
{
	VS_SHADOW Out;
	//座標変換

	float4x4 mat = mul(TransMatrix, ShadowProjection);
	Out.Pos = mul(Pos, mat);
	Out.Color = Out.Pos;
	return Out;
}

PS_OUTPUT_SHADOWMAP PS_ShadowBuf_MainStage(VS_SHADOW In)
{
	PS_OUTPUT_SHADOWMAP Out = (PS_OUTPUT_SHADOWMAP)0;
	Out.Default.r = In.Color.b / In.Color.w;
	Out.Default.g = Out.Default.r  * Out.Default.r ;
	Out.Default.b = .0f;
	Out.Default.a = .0f;

	Out.MainStage.r =1.0f;
	Out.MainStage.g = 1.0f  ;
	Out.MainStage.b = 1.0f;
	Out.MainStage.a = 1.0f;
	return Out;
}

technique ShadowBuf_MainStage
{
	pass Pass0
	{
		AlphaBlendEnable = false;
		ZWriteEnable = true;
		CullMode = CCW;

		VertexShader = compile vs_3_0 VS_ShadowBuf();
		PixelShader = compile ps_3_0 PS_ShadowBuf_MainStage();
	}
}
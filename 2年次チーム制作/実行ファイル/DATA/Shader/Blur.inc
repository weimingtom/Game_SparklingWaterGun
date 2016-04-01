

//ブラー
float offset_X;
float offset_Y;
float4 PS_Blur(VS_2D In) : COLOR
{
	float4 Out;
	Out = tex2D(DecaleSamp, In.Tex) * 0.2f;
	float4 p0 = tex2D(DecaleSamp, In.Tex + float2(-offset_X, -offset_Y)) * 0.1f;
		float4 p1 = tex2D(DecaleSamp, In.Tex + float2(.0f, -offset_Y)) * 0.1f;
		float4 p2 = tex2D(DecaleSamp, In.Tex + float2(offset_X, -offset_Y)) * 0.1f;

		float4 p3 = tex2D(DecaleSamp, In.Tex + float2(-offset_X, .0f)) * 0.1f;
		float4 p4 = tex2D(DecaleSamp, In.Tex + float2(offset_X, .0f)) * 0.1f;

		float4 p5 = tex2D(DecaleSamp, In.Tex + float2(-offset_X, offset_Y)) * 0.1f;
		float4 p6 = tex2D(DecaleSamp, In.Tex + float2(.0f, offset_Y)) * 0.1f;
		float4 p7 = tex2D(DecaleSamp, In.Tex + float2(offset_X, offset_Y)) * 0.1f;
		//足す
		Out = Out + p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7;
	Out.a = 1.0f;
	return Out;


}

float4 PS_gaussX(VS_2D In) : COLOR
{

	//テクセルを取得   
	float2 Texel0 = In.Tex + float2(-offset_X * 1, 0.0f);
	float2 Texel1 = In.Tex + float2(-offset_X * 2, 0.0f);
	float2 Texel2 = In.Tex + float2(-offset_X * 3, 0.0f);
	float2 Texel3 = In.Tex + float2(-offset_X * 4, 0.0f);
	float2 Texel4 = In.Tex + float2(-offset_X * 5, 0.0f);

	float2 Texel5 = In.Tex + float2(offset_X * 1, 0.0f);
	float2 Texel6 = In.Tex + float2(offset_X * 2, 0.0f);
	float2 Texel7 = In.Tex + float2(offset_X * 3, 0.0f);
	float2 Texel8 = In.Tex + float2(offset_X * 4, 0.0f);
	float2 Texel9 = In.Tex + float2(offset_X * 5, 0.0f);
	//取得したテクセル位置のカラー情報を取得する。
	//それぞれのカラー値に重みをかけている。この重み値はすべての合計が 1.0f になるように調整する。
	float4 p0 = tex2D(DecaleSamp, In.Tex) * 0.20f;

	float4 p1 = tex2D(DecaleSamp, Texel0) * 0.12f;
	float4 p2 = tex2D(DecaleSamp, Texel1) * 0.10f;
	float4 p3 = tex2D(DecaleSamp, Texel2) * 0.08f;
	float4 p4 = tex2D(DecaleSamp, Texel3) * 0.06f;
	float4 p5 = tex2D(DecaleSamp, Texel4) * 0.04f;

	float4 p6 = tex2D(DecaleSamp, Texel5) * 0.12f;
	float4 p7 = tex2D(DecaleSamp, Texel6) * 0.10f;
	float4 p8 = tex2D(DecaleSamp, Texel7) * 0.08f;
	float4 p9 = tex2D(DecaleSamp, Texel8) * 0.06f;
	float4 p10 = tex2D(DecaleSamp, Texel9) * 0.04f;

	//カラーを合成する
	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}

float4 PS_gaussY(VS_2D In) : COLOR
{
	//テクセルを取得   
	float2 Texel0 = In.Tex + float2(-offset_Y * 1, 0.0f);
	float2 Texel1 = In.Tex + float2(-offset_Y * 2, 0.0f);
	float2 Texel2 = In.Tex + float2(-offset_Y * 3, 0.0f);
	float2 Texel3 = In.Tex + float2(-offset_Y * 4, 0.0f);
	float2 Texel4 = In.Tex + float2(-offset_Y * 5, 0.0f);

	float2 Texel5 = In.Tex + float2(offset_Y * 1, 0.0f);
	float2 Texel6 = In.Tex + float2(offset_Y * 2, 0.0f);
	float2 Texel7 = In.Tex + float2(offset_Y * 3, 0.0f);
	float2 Texel8 = In.Tex + float2(offset_Y * 4, 0.0f);
	float2 Texel9 = In.Tex + float2(offset_Y * 5, 0.0f);

	//取得したテクセル位置のカラー情報を取得する。
	//それぞれのカラー値に重みをかけている。この重み値はすべての合計が 1.0f になるように調整する。
	float4 p0 = tex2D(DecaleSamp, In.Tex) * 0.20f;
	float4 p1 = tex2D(DecaleSamp, Texel0) * 0.12f;
	float4 p2 = tex2D(DecaleSamp, Texel1) * 0.10f;
	float4 p3 = tex2D(DecaleSamp, Texel2) * 0.08f;
	float4 p4 = tex2D(DecaleSamp, Texel3) * 0.06f;
	float4 p5 = tex2D(DecaleSamp, Texel4) * 0.04f;

	float4 p6 = tex2D(DecaleSamp, Texel5) * 0.12f;
	float4 p7 = tex2D(DecaleSamp, Texel6) * 0.10f;
	float4 p8 = tex2D(DecaleSamp, Texel7) * 0.08f;
	float4 p9 = tex2D(DecaleSamp, Texel8) * 0.06f;
	float4 p10 = tex2D(DecaleSamp, Texel9) * 0.04f;
	//カラーを合成する
	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9 + p10;
}

technique blur
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_2_0 PS_Blur();
	}
}

technique gaussX
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_2_0 PS_gaussX();
	}
}

technique gaussY
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_2_0 PS_gaussY();
	}
}

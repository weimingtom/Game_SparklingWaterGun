texture AnimeDistortion;
sampler AnimeDistortionSamp = sampler_state
{
	Texture = <AnimeDistortion>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

float4 PS_Distortion(VS_2D In) : COLOR
{
	float4	Out;
	//˜c‚ÝTexture‚ÌColor
	float4 distortion_Color = tex2D(AnimeDistortionSamp, In.Tex);
	if (distortion_Color.r >= 0.1f)
		Out = tex2D(DecaleSamp, In.Tex + float2(.0,-0.02f));
	else
		Out = tex2D(DecaleSamp, In.Tex);
	return Out;
}

technique distortion
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_3_0 PS_Distortion();
	}
}
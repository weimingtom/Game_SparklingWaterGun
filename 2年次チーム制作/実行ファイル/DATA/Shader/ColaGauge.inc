

struct VS_CLIP
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
	float2 Position	: TEXCOORD1;
};

texture Clipsampler;
sampler Clipsamp = sampler_state
{
	Texture = <Clipsampler>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
};

VS_CLIP VS_NoWork(VS_CLIP In)
{
	In.Position.xy=In.Pos.xy;
	return In;
}

float4 PS_Clip(VS_CLIP In) : COLOR
{
	float4	OUT;
	float2 tex = In.Tex;

	float2 ctex;//={0,0};

	ctex.x=In.Position.x*0.00078125;
	ctex.y=In.Position.y*0.00138888;

	//	ピクセル色決定
	OUT = In.Color * tex2D(DecaleSamp, tex);
	float4 clipcol=tex2D(Clipsamp,ctex);

	OUT*=clipcol;

	return OUT;
}

technique clip
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		VertexShader = compile vs_3_0 VS_NoWork();
		PixelShader = compile ps_2_0 PS_Clip();
	}
}

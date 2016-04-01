technique character
{
	//	ó÷äsê¸
	pass Outline
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CW;
		ZEnable = true;

		VertexShader = compile vs_3_0 VS_Outline();
		PixelShader = compile ps_3_0 PS_Outline();
	}

	//	ÉgÉDÅ[Éì
	pass Toon
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_3_0 VS_Toon();
		PixelShader = compile ps_3_0 PS_Toon();
	}
}

technique mainstage_lightmap
{
	pass Toon
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_3_0 VS_Toon();
		PixelShader = compile ps_3_0 PS_MainStage_LightMap();
	}
}

technique mainstage_realtime_shadow
{
	pass Toon
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_3_0 VS_Toon();
		PixelShader = compile ps_3_0 PS_MainStage_RealTimeShadow();
	}
}

technique cola_pool
{
	pass Toon
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_2_0 VS_TestPool();
		PixelShader = compile ps_2_0 PS_TestPool();
	}
}

technique see_through
{
	pass Toon
	{
		AlphaBlendEnable =	true;
		BlendOp          =	Add;
		SrcBlend         =	SrcAlpha;
		DestBlend        =	InvSrcAlpha;
		CullMode         =	CCW;
		ZEnable          =	false;

		VertexShader = compile vs_3_0 VS_See_Through();
		PixelShader = compile ps_3_0 PS_See_Through();
	}
}

technique truck
{
	pass Toon
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = CCW;
		ZEnable = true;

		VertexShader = compile vs_3_0 VS_Toon();
		PixelShader = compile ps_3_0 PS_Truck();
	}
}
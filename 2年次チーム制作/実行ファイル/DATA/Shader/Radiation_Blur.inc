
/**************放射ブラー関係****************/
float center_pos_x;
float center_pos_y;
float tex_offset_x;
float tex_offset_y;
float blur_power;

float blend_rate;

float4 PS_Radiation_Blur(VS_2D In) : COLOR
{
	float4 Color[10];
	//ブラーの中心位置→現在のテクセル位置
	float2 center = float2(center_pos_x, center_pos_y);

	float2 vec = center - In.Tex;
	float dist = length(vec);

	vec = normalize(vec) * float2(tex_offset_x, tex_offset_y);
	vec *=  dist * blur_power;
	Color[0] = tex2D(DecaleSamp, In.Tex) * 0.19f;
	Color[1] = tex2D(DecaleSamp, In.Tex + vec) * 0.17f;
	Color[2] = tex2D(DecaleSamp, In.Tex + vec * 2.0f) * 0.15f;
	Color[3] = tex2D(DecaleSamp, In.Tex + vec * 3.0f) * 0.13f;
	Color[4] = tex2D(DecaleSamp, In.Tex + vec * 4.0f) * 0.11f;
	Color[5] = tex2D(DecaleSamp, In.Tex + vec * 5.0f) * 0.09f;
	Color[6] = tex2D(DecaleSamp, In.Tex + vec * 6.0f) * 0.07f;
	Color[7] = tex2D(DecaleSamp, In.Tex + vec * 7.0f) * 0.05f;
	Color[8] = tex2D(DecaleSamp, In.Tex + vec * 8.0f) * 0.03f;
	Color[9] = tex2D(DecaleSamp, In.Tex + vec * 9.0f) * 0.01f;
	return Color[0] + Color[1] + Color[2] + Color[3] + Color[4] + Color[5] + Color[6] + Color[7] + Color[8] + Color[9];
}

technique radiation_blur
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		CullMode = None;
		ZEnable = false;

		PixelShader = compile ps_2_0 PS_Radiation_Blur();
	}
}
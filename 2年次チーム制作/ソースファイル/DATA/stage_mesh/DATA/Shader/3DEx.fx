
/*
�萔
*/

// �ϊ��s��
float4x4 TransMatrix;
float4x4 matProjection;
float4x4 MatView;

//���s���iLambert�j
float3 DirLight = {1.0f,.0f,.0f};

/*
�e�X�N�`���T���v��
*/

//	�����Ƀ��f���̕��ʂ��Ƃ̃e�N�X�`���������Ă���
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

//	�����Ƀ��f���̕��ʂ��Ƃ̃e�N�X�`���������Ă���
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
���_�V�F�[�_�[ (���_�̈ʒu�����[�J������X�N���[����Ɏ����Ă����V�F�[�_�[)
*/

VS_OUTPUT BasicVS(
	float4 pos			: POSITION,	//���_�̃��[�J���ʒu
	float3 normal	: NORMAL,	//���_�̃��[�J���ł̖@���̃x�N�g��
	float4 Color  : COLOR0,	//���_�̐F
	float2 Tex	  : TEXCOORD0//���_�̃e�N�X�`�����W
	){

	// ���_���W�A�e�N�X�`�����W
	VS_OUTPUT Out = (VS_OUTPUT)1;

	pos = mul(pos, TransMatrix);		//TransMatrix = "���[���h�ϊ�����"
	pos = mul(pos, MatView);		   //MatView="�J�����̎��_�Ɏ����Ă���"
	pos = mul(pos, matProjection);	//matProjection="��ʏ�Ɏ����Ă���"

	Out.Pos = pos;
	Out.Tex = Tex;

	//DirLight�֌W5
	//DirLight = normalize(DirLight);//���K��

	float3 world_normal = mul(normal, TransMatrix);
	world_normal = normalize(world_normal);

	float v=dot(-DirLight, world_normal);

	Out.Color.rgb = (v + 1)*0.5f;

	return Out;
}

/*5
�s�N�Z���V�F�[�_�[
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
�e�N�j�b�N
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
	float4 pos			: POSITION,	//���_�̃��[�J���ʒu
	float3 normal : NORMAL,	//���_�̃��[�J���ł̖@���̃x�N�g��
	float4 Color : COLOR0,	//���_�̐F
	float2 Tex : TEXCOORD0//���_�̃e�N�X�`�����W
	){

	// ���_���W�A�e�N�X�`�����W
	VS_OUTPUT Out = (VS_OUTPUT) 1;

	pos = mul ( pos, TransMatrix );		//TransMatrix = "���[���h�ϊ�����"
	pos = mul ( pos, MatView );		   //MatView="�J�����̎��_�Ɏ����Ă���"
	pos = mul ( pos, matProjection );	//matProjection="��ʏ�Ɏ����Ă���"

	Out.Pos = pos;
	Out.Tex = Tex;
	Out.Color.rgb = 1.0f;
	Out.Color.a = 0;

	return Out;
}

/*
�s�N�Z���V�F�[�_�[
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
	float4 pos			: POSITION,	//���_�̃��[�J���ʒu
	float3 normal : NORMAL,	//���_�̃��[�J���ł̖@���̃x�N�g��
	float4 Color : COLOR0,	//���_�̐F
	float2 Tex : TEXCOORD0//���_�̃e�N�X�`�����W
	)
{

	// ���_���W�A�e�N�X�`�����W
	VS_OUTPUT Out = (VS_OUTPUT) 1;

	pos.xyz += normalize ( normal ) * 1;

	pos = mul ( pos, TransMatrix );		//TransMatrix = "���[���h�ϊ�����"
	pos = mul ( pos, MatView );		   //MatView="�J�����̎��_�Ɏ����Ă���"
	pos = mul ( pos, matProjection );	//matProjection="��ʏ�Ɏ����Ă���"
	pos.z += 0.001f;

	Out.Pos = pos;
	Out.Tex = Tex;

	//DirLight�֌W5
	//	DirLight = normalize(DirLight);//���K��

	float3 world_normal = mul ( normal, ( float3x3 )TransMatrix );	//���[���h�ϊ������@��
		world_normal = normalize ( world_normal );

	float v = dot ( -DirLight, world_normal );

	Out.Color.rgb = ( v + 1 )*0.5f;

	return Out;
}

/*
�s�N�Z���V�F�[�_�[
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
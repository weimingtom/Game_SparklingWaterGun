texture Texture;
sampler DecaleSamp = sampler_state
{
    Texture = <Texture>;
    MinFilter = POINT;//LINEAR;
    MagFilter = POINT;//LINEAR;
    MipFilter = NONE;

    AddressU = CLAMP;
    AddressV = CLAMP;
};

sampler DecaleSamp2 = sampler_state
{
    Texture = <Texture>;
    MinFilter = GAUSSIANQUAD;
    MagFilter = GAUSSIANQUAD;
    MipFilter = NONE;

    AddressU = CLAMP;
    AddressV = CLAMP;
};


// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VS_OUTPUT_G
{
    float4 Pos			: POSITION;
	float2 Tex			: TEXCOORD0;
    float4 Color		: COLOR0;
};

// -------------------------------------------------------------
// ���_�V�F�[�_�v���O����
// -------------------------------------------------------------
VS_OUTPUT_G VS_pass1 (
      float4 Pos    : POSITION,          // ���f���̒��_
      float4 Tex    : TEXCOORD0,	         // �e�N�X�`�����W
      float4 Color  : COLOR0
){
    VS_OUTPUT_G Out;        // �o�̓f�[�^
    
    // �ʒu���W
    Out.Pos = Pos;
    Out.Tex = Tex;
    Out.Color = Color;
    return Out;
}

// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
// -------------------------------------------------------------
float4 PS_pass2(VS_OUTPUT_G In) : COLOR
{
	return In.Color * tex2D( DecaleSamp2, In.Tex );
}

float4 PS_pass1(VS_OUTPUT_G In) : COLOR
{   
    float4	Color;
    float4 temp;

	temp = tex2D( DecaleSamp, In.Tex );
	Color = temp*8;

	//	��	14t
	temp = tex2D( DecaleSamp, In.Tex + float2( + 0.002f, 0 )) + tex2D( DecaleSamp, In.Tex + float2( - 0.002f, 0 ) );
	Color += temp*4;
	
	temp = tex2D( DecaleSamp, In.Tex + float2( + 0.006f, 0 )) + tex2D( DecaleSamp, In.Tex + float2( - 0.005f, 0 ) );
	Color += temp*2;

	temp = tex2D( DecaleSamp, In.Tex + float2( + 0.01f, 0 )) + tex2D( DecaleSamp, In.Tex + float2( - 0.008f, 0 ) );
	Color += temp;

	//	�c	14t
	temp = tex2D( DecaleSamp, In.Tex + float2( 0, + 0.002f )) + tex2D( DecaleSamp, In.Tex + float2( 0, - 0.002f ) );
	Color += temp*4;
	
	temp = tex2D( DecaleSamp, In.Tex + float2( 0, + 0.006f )) + tex2D( DecaleSamp, In.Tex + float2( 0, - 0.005f ) );
	Color += temp*2;

	temp = tex2D( DecaleSamp, In.Tex + float2( 0, + 0.01f )) + tex2D( DecaleSamp, In.Tex + float2( 0, - 0.008f ) );
	Color += temp;


	//	�㉡	8t	
	temp = tex2D( DecaleSamp, In.Tex + float2( + 0.002f, -0.002f )) + tex2D( DecaleSamp, In.Tex + float2( - 0.002f, -0.002f ) );
	Color += temp*4;
	
	//	����	8t
	temp = tex2D( DecaleSamp, In.Tex + float2( + 0.002f, +0.002f )) + tex2D( DecaleSamp, In.Tex + float2( - 0.002f, +0.002f ) );
	Color += temp*4;
	
	Color /= 52;
	
    return Color;
}
// -------------------------------------------------------------
// �e�N�j�b�N
// -------------------------------------------------------------
technique gauss
{
    pass P0
    {
 		AlphaBlendEnable = false;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = Zero;
		// �V�F�[�_
        VertexShader = compile vs_3_0 VS_pass1();
        PixelShader  = compile ps_3_0 PS_pass1();
    }
}

technique copy
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		// �V�F�[�_
        VertexShader = compile vs_3_0 VS_pass1();
        PixelShader  = compile ps_3_0 PS_pass2();
    }
}

technique add
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = one;
		// �V�F�[�_
        VertexShader = compile vs_3_0 VS_pass1();
        PixelShader  = compile ps_3_0 PS_pass2();
    }
}


//------------------------------------------------------
//		�X�N���[���t�B���^
//------------------------------------------------------
float	contrast = 1.8;
float	chroma	= .9f;
float3	ScreenColor = { 1.0f, 1.9f, 1.1f };

struct VS_M
{
    float4 Pos		: POSITION;
    float4 Color	: COLOR0;
    float2 Tex		: TEXCOORD0;
};

//------------------------------------------------------
//		��{���_�V�F�[�_�[	
//------------------------------------------------------
VS_M VS_Master( VS_M In ){
	return In;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_Master( VS_M In) : COLOR
{   
	float4	OUT;

	float3	col = tex2D( DecaleSamp, In.Tex );

	col = ((col-0.5f)*contrast) + 0.5f;

	float	avr = (col.r + col.g + col.b) / 3;
	col = (col-avr) * chroma + avr;

	//	�s�N�Z���F����
	OUT.rgb = col * ScreenColor;
	OUT.w = 1;
	
	return OUT;
}


//------------------------------------------------------
//
//------------------------------------------------------
technique Mastering
{
    pass P0
    {
		AlphaBlendEnable = false;
		BlendOp          = Add;
		SrcBlend         = one;
		DestBlend        = zero;

		VertexShader = compile vs_2_0 VS_Master();
		PixelShader  = compile ps_2_0 PS_Master();
    }
}


//------------------------------------------------------
//		�p�[�e�B�N��
//------------------------------------------------------

float3 LightDir = { 0.707f,0.707f,-0.707f };

texture NormalMap;	//	�@���}�b�v�e�N�X�`��
sampler NormalSamp = sampler_state
{
    Texture = <NormalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

//------------------------------------------------------
//		��{���_�V�F�[�_�[	
//------------------------------------------------------
VS_M VS_Particle( VS_M In ){
	return In;
}

//------------------------------------------------------
//		�s�N�Z���V�F�[�_�[	
//------------------------------------------------------
float4 PS_Particle( VS_M In) : COLOR
{   
	float4	OUT;

	//	���C�g�v�Z
	float3 N = tex2D( NormalSamp, In.Tex ).xyz * 2.0f - 1.0f;
	float rate = max( 0, dot( -LightDir, N ) );
	float4	col = tex2D( DecaleSamp2, In.Tex );
	//	�s�N�Z���F����
	OUT = In.Color * col;
	OUT.rgb *= rate;
	
	return OUT;
}


//------------------------------------------------------
//
//------------------------------------------------------
technique Particle
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		ZWriteEnable     = false;

		VertexShader = compile vs_3_0 VS_Particle();
		PixelShader  = compile ps_3_0 PS_Particle();
    }
}


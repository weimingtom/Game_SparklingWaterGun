float4x4 Projection;	//	���e�ϊ��s��
float4x4 TransMatrix;
float4x4 matView;
float4x4 matProjection;

float Zfar;
//�Ƃ肠����
float3	LightDir;
float3		Camera_Pos;
float	AdjustValue = -0.001f;
float	ShadowLevel = 0.8f;
float specPower = 1.2f;

float OutlineSize = .06f;	//�֊s���̑���
float outlinecolor_R = .0f;
float outlinecolor_G = .0f;
float outlinecolor_B = .0f;
float outlinecolor_A = 1.0f;


/**************�g�D�[���e****************/
float	ToonLevel = 0.4f;		//�e�̃��x��
float ToonShadow = 0.8f;	//�e�̔Z��
float4x4 n_ShadowProjection;	//	�ߋ���
float4x4 f_ShadowProjection;	//������

float3 colacolor = { 189.0f / 255.0, 33.0f / 255.0, 33.0f / 255.0 };
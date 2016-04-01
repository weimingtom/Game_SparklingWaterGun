#include	"iExtreme.h"

//*****************************************************************************
//
//		�Q�c�I�u�W�F�N�g
//
//*****************************************************************************
//------------------------------------------------------
//	�R���X�g���N�^
//------------------------------------------------------
iex2DObj::iex2DObj( char* filename )
{
	//	��񏉊���
	width = height = 0;
	lpSurface = NULL;

	//	�e�N�X�`���ǂݍ���
	lpTexture = iexTexture::Load(filename);
	
	if (lpTexture == nullptr)
	{
		MyAssert(lpTexture, "iex2DObj�̓ǂݍ��݂Ɏ��s���܂����B�t�@�C���� %s \n", filename);
		return;
	}
	//	�T�C�Y�ۑ�
	D3DSURFACE_DESC	sd;
	lpTexture->GetLevelDesc( 0, &sd );
	width  = sd.Width;
	height = sd.Height;
	//	���C���T�[�t�F�X�擾
	lpTexture->GetSurfaceLevel( 0, &lpSurface );
}

iex2DObj::iex2DObj( u32 width, u32 height, u8 flag )
{
	//	��񏉊���
	this->width = this->height = 0;
	lpSurface = NULL;

	D3DFORMAT	fmt = D3DFMT_R8G8B8;
	u32			usage = 0;
	D3DPOOL		pool = D3DPOOL_MANAGED;

	switch( flag ){
	case IEX2D_RENDERTARGET:	//	�����_�[�\
					usage = D3DUSAGE_RENDERTARGET;
					fmt = iexSystem::ScreenFormat;
					pool = D3DPOOL_DEFAULT;
					break;
	case IEX2D_FLOAT2:	//	�����_�[�\
					usage = D3DUSAGE_RENDERTARGET;
					fmt = D3DFMT_G32R32F;
					pool = D3DPOOL_DEFAULT;
					break;
	case IEX2D_FLOAT:	//	�����_�[�\
					usage = D3DUSAGE_RENDERTARGET;
					fmt = D3DFMT_R32F;
					pool = D3DPOOL_DEFAULT;
					break;

	case IEX2D_USEALPHA:		//	�����x�t
					fmt = D3DFMT_A8R8G8B8;
					break;
	case IEX2D_SYSTEMMEM:
					fmt = iexSystem::ScreenFormat;
					pool = D3DPOOL_SYSTEMMEM;
					break;
	}
	dwFlags= 0;
	//	�e�N�X�`���쐬
	D3DXCreateTexture( iexSystem::Device, width, height, 0, usage, fmt, pool, &lpTexture );
	if( lpTexture == NULL ) return;

	//	�T�C�Y�ۑ�
	D3DSURFACE_DESC	sd;
	lpTexture->GetLevelDesc( 0, &sd );
	this->width  = sd.Width;
	this->height = sd.Height;
	//	���C���T�[�t�F�X�擾
	lpTexture->GetSurfaceLevel( 0, &lpSurface );
}

//*****************************************************************************
//	�����_�[�^�[�Q�b�g�ݒ�
//*****************************************************************************
void	iex2DObj::RenderTarget( int index )
{
	iexSystem::Device->SetRenderTarget( index,lpSurface );
}


//*****************************************************************************
//	�����_�����O
//*****************************************************************************

void iex2DObj::Render()
{
	TLVERTEX	v[4];
	//	�S��ʐݒ�
	v[0].sx = v[2].sx = 0;
	v[1].sx = v[3].sx = iexSystem::ScreenWidth - 0.5f;
	v[0].sy = v[1].sy = 0;
	v[2].sy = v[3].sy = iexSystem::ScreenHeight - 0.5f;

	v[0].tu = v[2].tu = 0;
	v[1].tu = v[3].tu = 1;
	v[0].tv = v[1].tv = 0;
	v[2].tv = v[3].tv = 1;

	v[0].color = v[1].color = v[2].color = v[3].color = 0xFFFFFFFF;
	v[0].sz    = v[1].sz    = v[2].sz    = v[3].sz    = 0;
	v[0].rhw   = v[1].rhw   = v[2].rhw   = v[3].rhw   = 1.0f;

	iexPolygon::Render2D( v, 2, this, RS_COPY );
}

void iex2DObj::Render( iexShader* shader, char* tech )
{
	TLVERTEX	v[4];
	//	�S��ʐݒ�
	v[0].sx = v[2].sx = 0;
	v[1].sx = v[3].sx = iexSystem::ScreenWidth - 0.5f;
	v[0].sy = v[1].sy = 0;
	v[2].sy = v[3].sy = iexSystem::ScreenHeight - 0.5f;

	v[0].tu = v[2].tu = 0;
	v[1].tu = v[3].tu = 1;
	v[0].tv = v[1].tv = 0;
	v[2].tv = v[3].tv = 1;

	v[0].color = v[1].color = v[2].color = v[3].color = 0xFFFFFFFF;
	v[0].sz    = v[1].sz    = v[2].sz    = v[3].sz    = 0;
	v[0].rhw   = v[1].rhw   = v[2].rhw   = v[3].rhw   = 1.0f;

	iexPolygon::Render2D( v, 2, this, shader, tech );
}

void iex2DObj::Render( s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, u32 dwFlags, COLOR color, float z )
{
	TLVERTEX	v[4];
	v[0].sx = v[2].sx = (float)DstX;
	v[1].sx = v[3].sx = (float)(DstX+DstW)-0.5f;
	v[0].sy = v[1].sy = (float)DstY;
	v[2].sy = v[3].sy = (float)(DstY+DstH)-0.5f;

	v[0].tu = v[2].tu = (float)(SrcX+0.5f)/(float)this->width;
	v[1].tu = v[3].tu = (float)(SrcX+width)/(float)this->width;
	v[0].tv = v[1].tv = (float)(SrcY+0.5f)/(float)this->height;
	v[2].tv = v[3].tv = (float)(SrcY+height)/(float)this->height;

	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].sz    = v[1].sz    = v[2].sz    = v[3].sz    = z;
	v[0].rhw   = v[1].rhw   = v[2].rhw   = v[3].rhw   = 1.0f;

	iexPolygon::Render2D( v, 2, this, dwFlags );
}

void iex2DObj::Render( s32 DstX, s32 DstY, s32 DstW, s32 DstH, s32 SrcX, s32 SrcY, s32 width, s32 height, iexShader* shader, char* tech, COLOR color, float z )
{
	TLVERTEX	v[4];
	v[0].sx = v[2].sx = (float)DstX;
	v[1].sx = v[3].sx = (float)(DstX+DstW)-0.5f;
	v[0].sy = v[1].sy = (float)DstY;
	v[2].sy = v[3].sy = (float)(DstY+DstH)-0.5f;

	v[0].tu = v[2].tu = (float)(SrcX+0.5f)/(float)this->width;
	v[1].tu = v[3].tu = (float)(SrcX+width)/(float)this->width;
	v[0].tv = v[1].tv = (float)(SrcY+0.5f)/(float)this->height;
	v[2].tv = v[3].tv = (float)(SrcY+height)/(float)this->height;

	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].sz    = v[1].sz    = v[2].sz    = v[3].sz    = z;
	v[0].rhw   = v[1].rhw   = v[2].rhw   = v[3].rhw   = 1.0f;

	iexPolygon::Render2D( v, 2, this, shader, tech );
}

void iex2DObj::Render(
	CRTextureRenderRect render_rect,
	u32 dwFlags, 
	COLOR color ,
	float z )
{
	TLVERTEX	v[4];
	v[0].sx = v[2].sx = (float)render_rect.render_rect.left;
	v[1].sx = v[3].sx = (float)(render_rect.render_rect.left + render_rect.render_rect.width) - 0.5f;
	v[0].sy = v[1].sy = (float)render_rect.render_rect.top;
	v[2].sy = v[3].sy = (float)(render_rect.render_rect.top + render_rect.render_rect.height) - 0.5f;

	v[0].tu = v[2].tu = (float)(render_rect.read_tex_rect.left + 0.5f) / (float)this->width;
	v[1].tu = v[3].tu = (float)(render_rect.read_tex_rect.left + render_rect.read_tex_rect.width) / (float)this->width;
	v[0].tv = v[1].tv = (float)(render_rect.read_tex_rect.top + 0.5f) / (float)this->height;
	v[2].tv = v[3].tv = (float)(render_rect.read_tex_rect.top + render_rect.read_tex_rect.height) / (float)this->height;

	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].sz = v[1].sz = v[2].sz = v[3].sz = z;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;

	iexPolygon::Render2D(v, 2, this, dwFlags);
}

void iex2DObj::Render(
	const Vector2& screen_left_up_pos,
	const Vector2& screen_render_size,
	const Vector2& texture_left_up_pos,
	const Vector2& texture_render_size,
	DWORD	dwFlags,
	COLOR color,
	float z
	)
{
	TLVERTEX	v[4];
	v[0].sx = v[2].sx = (float)screen_left_up_pos.x;
	v[1].sx = v[3].sx = (float)(screen_left_up_pos.x + screen_render_size.x);
	v[0].sy = v[1].sy = (float)screen_left_up_pos.y;
	v[2].sy = v[3].sy = (float)(screen_left_up_pos.y + screen_render_size.y);

	v[0].tu = v[2].tu = texture_left_up_pos.x;
	v[1].tu = v[3].tu = texture_left_up_pos.x + texture_render_size.x;
	v[0].tv = v[1].tv = texture_left_up_pos.y;
	v[2].tv = v[3].tv = texture_left_up_pos.y + texture_render_size.y;

	v[0].color = v[1].color = v[2].color = v[3].color = color;
	v[0].sz = v[1].sz = v[2].sz = v[3].sz = z;
	v[0].rhw = v[1].rhw = v[2].rhw = v[3].rhw = 1.0f;

	iexPolygon::Render2D(v, 2, this, dwFlags);
}

//��]�t���`��
void iex2DObj::RenderPlus(
	const Vector2& screen_center_pos,	//�`�悷�钆�S�ʒu
	const Vector2& screen_size,			//�`�悷��傫��
	float angle,						//��]�p�x
	const Vector2& texture_left_up_pos,	//�摜��̍���̍��W
	const Vector2& texture_render_size,	//���ォ��ǂꂾ���ǂݍ��ނ�
	DWORD	dwFlags,					//�`����@
	COLOR color,						//���_�J���[
	float z								//���_���l
	)
{
	const Vector2 screen_render_size(screen_size.x / 2, screen_size.y/2);

	TLVERTEX	v[4]=
	{
		{ -screen_render_size.x, -screen_render_size.y, z, 1, color },
		{ screen_render_size.x, -screen_render_size.y, z, 1, color },
		{ -screen_render_size.x, screen_render_size.y, z, 1, color },
		{ screen_render_size.x, screen_render_size.y, z, 1, color }
	};

	if (angle != 0)
	{
		const float sinA = sinf(angle), cosA = cosf(angle);
		
		v[0].sx = -screen_render_size.x*cosA + -screen_render_size.y*sinA;
		v[0].sy = -screen_render_size.x*-sinA + -screen_render_size.y*cosA;
		
		v[1].sx = screen_render_size.x*cosA + -screen_render_size.y*sinA;
		v[1].sy = screen_render_size.x*-sinA + -screen_render_size.y*cosA;

		v[2].sx = -v[1].sx;
		v[2].sy = -v[1].sy;

		v[3].sx = -v[0].sx;
		v[3].sy = -v[0].sy;
	}

	for (int i = 0; i < 4; i++)
	{
		v[i].sx += screen_center_pos.x;
		v[i].sy += screen_center_pos.y;
	}

	v[0].tu = v[2].tu = (float)(texture_left_up_pos.x) / (float)this->width;
	v[1].tu = v[3].tu = (float)(texture_left_up_pos.x + texture_render_size.x) / (float)this->width;
	v[0].tv = v[1].tv = (float)(texture_left_up_pos.y) / (float)this->height;
	v[2].tv = v[3].tv = (float)(texture_left_up_pos.y + texture_render_size.y) / (float)this->height;

	iexPolygon::Render2D(v, 2, this, dwFlags);
}

//**************************************************************************************************
//
//		�ߋ���IEX�Ƃ̌݊��p
//
//**************************************************************************************************

void	IEX_Render2DObject( int DstX, int DstY, int DstW, int DstH, LPIEX2DOBJ lpObj, int SrcX, int SrcY, int width, int height, u32 dwFlags, u32 color )
{
	if( !lpObj ) return;
	lpObj->Render(DstX, DstY, DstW, DstH, SrcX, SrcY, width, height, dwFlags, color );
}


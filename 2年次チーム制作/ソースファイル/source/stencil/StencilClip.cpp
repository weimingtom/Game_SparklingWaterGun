#include "StencilClip.h"

StencilClip::StencilClip() :
device_(0),
mode_(Mode_Masking),
writeColor_(MaskColor_Fill),
refColor_(MaskColor_Fill),
curZTest_(0),
curZFunc_(0)
{
}

StencilClip::~StencilClip() 
{
}

//! クリッピング領域作成開始
bool StencilClip::regionBegin( IDirect3DDevice9* device, MaskColor initColor ) 
{
	device_ = device;

	if ( device_ == 0 ) {
		return false;
	}

	// ステンシルバッファのみ初期値でクリア
	device->Clear( 0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, static_cast<DWORD>( initColor ) );

	// 既存のZテストパラメータを保存
	device->GetRenderState( D3DRS_ZENABLE, &curZTest_ );
	device->GetRenderState( D3DRS_ZFUNC,   &curZFunc_ );

	// ステンシルバッファを有効化
	// カラーの書き込みは阻止したいので
	// Zテストをすべて失敗させるがステンシルバッファへの書き込みは許可する
	device->SetRenderState( D3DRS_ZENABLE, true );
	device->SetRenderState( D3DRS_ZFUNC, D3DCMP_NEVER );

	device->SetRenderState( D3DRS_STENCILENABLE, true );
	device->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_ALWAYS );
	device->SetRenderState( D3DRS_STENCILPASS,	D3DSTENCILOP_REPLACE );
	device->SetRenderState( D3DRS_STENCILZFAIL,	D3DSTENCILOP_REPLACE );
	device->SetRenderState( D3DRS_STENCILREF,	writeColor_ );
	device->SetRenderState( D3DRS_STENCILMASK, 0xff );

	mode_ = Mode_Masking;

	return true;
}

//! クリッピング領域作成終了
bool StencilClip::regionEnd() 
{
	if ( device_ == 0 ) {
		return false;
	}

	// ステンシルを一時無効化
	// Zテストを戻す
	device_->SetRenderState( D3DRS_STENCILENABLE, false );
	device_->SetRenderState( D3DRS_ZENABLE, curZTest_ );
	device_->SetRenderState( D3DRS_ZFUNC, curZFunc_ );

	return true;
}

//! クリッピング描画開始
bool StencilClip::drawBegin() 
{
	if ( device_ == 0 ) {
		return false;
	}

	// すでに書き込まれているステンシル値に対して
	// マスク色でない部分だけテスト合格にする
	device_->SetRenderState( D3DRS_STENCILENABLE, true );
	device_->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL );
	device_->SetRenderState( D3DRS_STENCILPASS,	D3DSTENCILOP_KEEP );
	device_->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
	device_->SetRenderState( D3DRS_STENCILREF, refColor_ );
	device_->SetRenderState( D3DRS_STENCILMASK, 0xff );

	mode_ = Mode_Draw;

	return true;
}

//! クリッピング描画終了
bool StencilClip::drawEnd( MaskColor clearColor ) 
{
	if ( device_ == 0 ) {
		return false;
	}

	// ステンシルを無効化しバッファをクリア
	device_->SetRenderState( D3DRS_STENCILENABLE, false );
	device_->Clear( 0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, static_cast<DWORD>( clearColor ) );

	return true;
}

//! 書き込み時マスクカラーの設定
void StencilClip::setWriteMaskColor( MaskColor color ) 
{
	writeColor_ = color;
	if ( device_ && mode_ == Mode_Masking ) {
		device_->SetRenderState( D3DRS_STENCILREF, writeColor_ );
	}
}

//! 描画時のマスクカラーの指定
void StencilClip::setRefMaskColor(MaskColor color) 
{
	refColor_ = color;
	if (device_ && mode_ == Mode_Draw) {
		device_->SetRenderState(D3DRS_STENCILREF, refColor_);
	}
}